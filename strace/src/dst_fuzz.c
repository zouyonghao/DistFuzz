#include "dst_fuzz.h"
#include "ptrace.h"
#include "regs.h"
#include "scno.h"
#include "delay.h"
#include "retval.h"
#include "utils/dst_share_mem_util.h"
#include "dst_random.h"
#include "unwind.h"

#ifdef ENABLE_KAFKA
#include "librdkafka/rdkafka.h"
#endif

#include <sys/uio.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/shm.h>

/*
 * max delay millseconds 1000ms
 * TODO: make it configurable
 */
#define MAX_DELAY_MILLSECONDS 1000

#define FUZZ_COVERAGE_MAP_ENV_ID "AFLCplusplus_BRANCH_TRACE_SHM_ID"

#define FUZZ_COVERAGE_MAP_SIZE (1u << 23)

struct exec_params {
	int fd_to_close;
	uid_t run_euid;
	gid_t run_egid;
	char **argv;
	char **env;
	char *pathname;
	struct sigaction child_sa;
};
extern struct exec_params params_for_tracee;

static char *pathname;

/**
 * According to https://man7.org/linux/man-pages/man2/_syscall.2.html,
 * the raw syscall returns the negative of a standard error code.
 * So we need to specify all the possible error codes for read/write/recv/send.
 * NOTE: eventfd can only get EAGAIN according to https://man7.org/linux/man-pages/man2/eventfd.2.html
 */
int READ_ERROR_CODES[] = { EAGAIN, EWOULDBLOCK, EBADF, EFAULT, EINTR, EINVAL, EIO };
int WRITE_ERROR_CODES[] = { EAGAIN, EWOULDBLOCK, EBADF, EDESTADDRREQ, EDQUOT, EFAULT, EFBIG,
			    EINTR,  EINVAL,	 EIO,	ENOSPC,	      EPERM,  EPIPE };

int RECV_ERROR_CODES[] = { EAGAIN, EWOULDBLOCK, EBADF,	ECONNREFUSED, EFAULT,
			   EINTR,  EINVAL,	ENOMEM, ENOTCONN,     ENOTSOCK };
int SEND_ERROR_CODES[] = { EACCES, EAGAIN,   EALREADY, EBADF,	   ECONNRESET, EDESTADDRREQ,
			   EFAULT, EINTR,    EINVAL,   EISCONN,	   EMSGSIZE,   ENOBUFS,
			   ENOMEM, ENOTCONN, ENOTSOCK, EOPNOTSUPP, EPIPE };

bool is_dst_fuzz = false;

struct user_regs_struct *stored_regs;

static bool printed_fuzzing_stopped = false;

static bool last_injected = false;

#ifdef ENABLE_KAFKA
bool use_kafka = false;
#ifndef KAFKA_BOOTSTRAP_SERVER
#define KAFKA_BOOTSTRAP_SERVER "control:9092" // jepsen control node
#endif
#define KAFKA_COVERAGE_TOPIC "coverage"
rd_kafka_topic_t *topic_coverage = NULL;
void init_kafka_producer(void);
#endif

void init_dst_fuzz(void);

void handle_random_event(struct tcb *tcp, bool is_send, size_t length, int error_codes[],
			 int error_codes_length);

int compare_and_warning_regs_differences(const struct user_regs_struct *const new,
					 const struct user_regs_struct *const old);
int get_file_path(struct tcb *tcp, int fd, char **buf);

uint64_t get_length_from_iovec(struct tcb *tcp, int iov_pos, int iovcnt_pos);
uint64_t get_length_from_msg(struct tcb *tcp, int msg_pos);
uint64_t get_length_from_mmsg(struct tcb *tcp, int msgvec_pos, int vlen_pos);

uint64_t get_hash_value(bool is_send, uint64_t position, size_t length, bool fault_injected);

// static int arch_set_error(struct tcb *);
// #include "arch_regs.c"
// #include "set_error.c"

static uint8_t *fuzz_coverage_map;

static int ignore_range = 1;

// temp variables, should be restored after each syscall finished
static unsigned long tmp_offset = 0;

static uint64_t event_index = 0;
uint64_t
get_hash_value(bool is_send, uint64_t position, size_t length, bool fault_injected)
{
	int node_id = getenv("NODE_ID") != NULL ? atoi(getenv("NODE_ID")) : 0;
	uint64_t hash_value = ((event_index + is_send * 10 + length + node_id * 100 + position)
			       << fault_injected) %
			      FUZZ_COVERAGE_MAP_SIZE;
	for (int i = 1; i <= ignore_range; i++) {
		if (fuzz_coverage_map[hash_value - i] > 0) {
			return hash_value - i;
		}
		if (fuzz_coverage_map[hash_value + 1] > 0) {
			return hash_value + i;
		}
	}
	event_index++;
	return hash_value;
}

static void
print_call_cb(void *dummy, const char *binary_filename, const char *symbol_name,
	      unwind_function_offset_t function_offset, unsigned long true_offset)
{
	if (tmp_offset == 0 && strstr(binary_filename, pathname)) {
		// fprintf(stderr, "pathname is %s\n", pathname);
		// fprintf(stderr, "true_offset is %ld\n", true_offset);
		tmp_offset = true_offset;
	}
}

static void
print_error_cb(void *dummy, const char *error, unsigned long true_offset)
{
}

void
handle_random_event(struct tcb *tcp, bool is_send, size_t length, int error_codes[],
		    int error_codes_length)
{
	/*
	 * stored_regs != NULL means we want to call a syscall again, so we
	 * should set it back and skip the random event to let it be called.
	 */
	if (stored_regs != NULL) {
		free(stored_regs);
		stored_regs = NULL;
		return;
	}

	/* If fuzzing is disabled, we call the original function. */
	if (!get_is_fuzzing()) {
		if (!printed_fuzzing_stopped) {
			fprintf(stderr,
				"strace: fuzzing is stopped, will use kernel_func directly.\n");
			printed_fuzzing_stopped = true;
		}
		return;
	}

	/* Ignore stdin, stdout, stderr */
	if (tcp->u_arg[0] < 3) {
		return;
	}

	int fd = tcp->u_arg[0];

	char *file_path;
	if (get_file_path(tcp, fd, &file_path) == -1) {
		perror("get_file_path:");
		return;
	}

	bool is_file = false;
	bool is_eventfd = false;
	switch (file_path[0]) {
	case 's': /* socket: [xxxx] */
		// fprintf(stderr, "fd %d in process %d is a socket\n", fd, tcp->pid);
		break;

	case 'p': /* pipe:[xxxx] */
		return;

	default:
		if (strstr(file_path, ".o") != NULL) {
			return;
		}
		if (strstr(file_path, ".so") != NULL) {
			return;
		}
		if (strstr(file_path, ".cfg") != NULL) {
			return;
		}
		if (strstr(file_path, "lib") != NULL) {
			return;
		}
		if (strstr(file_path, "/sys") != NULL) {
			return;
		}
		if (strstr(file_path, "/usr") != NULL) {
			return;
		}
		if (strstr(file_path, "/proc") != NULL) {
			return;
		}
		if (strstr(file_path, "/dev") != NULL) {
			return;
		}
		if (strstr(file_path, "/etc") != NULL) {
			return;
		}
		if (strstr(file_path, ".properties") != NULL) {
			return;
		}
		/* Skip jars for java applications */
		if (strstr(file_path, ".jar") != NULL) {
			return;
		}

		if (strstr(file_path, "[eventfd]") != NULL) {
			is_eventfd = true;
		}

		/* Skip files in get_no_fault_files */
		const char *no_fault_files_env = getenv("NO_FAULT_FILES");
		if (no_fault_files_env != NULL && strlen(no_fault_files_env) > 0) {
			int files_length = strlen(no_fault_files_env);
			char *no_fault_files = malloc(files_length + 1);
			strcpy(no_fault_files, no_fault_files_env);
			fprintf(stderr, "no_fault_files is %s\n", no_fault_files);
			const char *delim = ",";
			char *no_fault_file = strtok(no_fault_files, delim);
			while (no_fault_file != NULL) {
				if (strstr(file_path, no_fault_file) != NULL) {
					fprintf(stderr, "%s is skipped\n", no_fault_file);
					return;
				}
				no_fault_file = strtok(NULL, delim);
			}
			free(no_fault_files);
		}

		is_file = true;
		// fprintf(stderr, "fd %d in process %d is a file\n", fd, tcp->pid);
		break;
	}

	// NOTE: The use of IP register is not suitable
	//       because every syscall has same offset in libc
	// struct user_regs_struct _regs;
	// ptrace(PTRACE_GETREGS, tcp->pid, NULL, &_regs);
	// fprintf(stderr, "rip - fs_base is %llX\n", _regs.rip - _regs.fs_base);
	unwinder.tcb_walk(tcp, print_call_cb, print_error_cb, NULL);

	bool fault_injected = false;

	if (getenv("NO_FAULT") != NULL) {
		goto EXIT;
	}

	uint8_t random = __dst_get_random_uint8_t();
	// fprintf(stderr, "random is %u\n", random);
	if (random < 50) {
		goto EXIT;
	}

	fault_injected = true;

	fprintf(stderr, "get file path %s\n", file_path);

	last_injected = true;

	/*
	 * strace uses the inject_vec[current_personality] which is also a list of 
	 * inject_opts so we must alloc a list of inject_opts here which wastes much memory.
	 * 
	 * TODO: optimize the memory usage.
	 */
	struct inject_opts *opts = malloc(sizeof(struct inject_opts) * (tcp->scno + 1));
	opts[tcp->scno].first = 1;
	opts[tcp->scno].last = (uint16_t)-1;
	opts[tcp->scno].step = 1;
	opts[tcp->scno].data.flags = 0;
	opts[tcp->scno].data.signo = 0;
	opts[tcp->scno].data.rval_idx = (uint16_t)-1;
	opts[tcp->scno].data.delay_idx = (uint16_t)-1;
	opts[tcp->scno].data.scno = 0;
	tcp->inject_vec[current_personality] = opts;

	enum INJECT_EVENT events[] = {
		DELAY, FAIL,
		/* be careful of LOST and DUP, this may leads to false positive because of partial write of a message. */
		LOST, DUP
	};
	int supported_event_size = 4;
	// int supported_event_size = 2;
	/* read socket, read file, write file can noly DELAY or FAIL */
	if (!is_send || is_file) {
		supported_event_size = 2;
	}

	switch (events[__dst_get_random_uint8_t() % supported_event_size]) {
	case DELAY: {
		tcp->qual_flg |= QUAL_INJECT;
		tcp->flags |= INJECT_F_DELAY_ENTER;
		opts[tcp->scno].data.flags = INJECT_F_DELAY_ENTER;
		opts[tcp->scno].data.delay_idx = alloc_delay_data();
		struct timespec tsval;
		tsval.tv_sec = 0;
		/** sleep for 0s ~ 1s */
		uint16_t random = __dst_get_random_uint16_t() % MAX_DELAY_MILLSECONDS;
		tsval.tv_nsec = random * 1e6;
		fill_delay_data(opts[tcp->scno].data.delay_idx, &tsval, true);
		fprintf(stderr, "inject delay to process %d syscall %ld\n", tcp->pid, tcp->scno);
		break;
	}
	case FAIL: {
		tcp->qual_flg |= QUAL_INJECT;
		tcp->flags |= INJECT_F_RETVAL;
		opts[tcp->scno].data.flags = INJECT_F_RETVAL;
		kernel_long_t ret_val = 0;
		if (is_file) {
		}
		if (is_eventfd) {
			ret_val = EAGAIN;
		} else {
			ret_val = error_codes[__dst_get_random_uint8_t() % error_codes_length];
		}
		opts[tcp->scno].data.rval_idx = retval_new(-ret_val);
		fprintf(stderr, "inject fail to process %d syscall %ld\n", tcp->pid, tcp->scno);
		break;
	}
	case LOST: {
		tcp->qual_flg |= QUAL_INJECT;
		tcp->flags |= INJECT_F_RETVAL;
		opts[tcp->scno].data.flags = INJECT_F_RETVAL;
		opts[tcp->scno].data.rval_idx = retval_new(length);
		fprintf(stderr, "inject lost to process %d syscall %ld\n", tcp->pid, tcp->scno);
		break;
	}
	case DUP: {
		stored_regs = malloc(sizeof(struct user_regs_struct));
		ptrace(PTRACE_GETREGS, tcp->pid, NULL, stored_regs);
		fprintf(stderr, "we should dup this syscall, stored rip is %lld\n",
			stored_regs->rip);
		fprintf(stderr, "inject dup to process %d syscall %ld\n", tcp->pid, tcp->scno);
		break;
	}
	}

EXIT:
	uint64_t hash_index = get_hash_value(is_send, tmp_offset, length, fault_injected);
#ifdef ENABLE_KAFKA
	if (use_kafka) {
		if (rd_kafka_produce(topic_coverage, RD_KAFKA_PARTITION_UA, RD_KAFKA_MSG_F_COPY,
				     &hash_index, sizeof(uint64_t), NULL, 0, NULL) == -1) {
			fprintf(stderr, "%% Failed to produce to topic %s: %s\n",
				KAFKA_COVERAGE_TOPIC, rd_kafka_err2str(rd_kafka_last_error()));
		}
	} else {
#endif
		fuzz_coverage_map[hash_index]++;
#ifdef ENABLE_KAFKA
	}
#endif
}

void
dst_fuzz_syscall_enter(struct tcb *tcp)
{
	if (!is_dst_fuzz) {
		init_dst_fuzz();
	}
	if (!tcp->inject_vec[current_personality]) {
		tcp->inject_vec[current_personality] =
			xarraydup(inject_vec[current_personality], nsyscalls, sizeof(**inject_vec));
	}
	switch (tcp->scno) {
	case __NR_read:
		handle_random_event(tcp, false, tcp->u_arg[2], READ_ERROR_CODES,
				    sizeof(READ_ERROR_CODES) / sizeof(READ_ERROR_CODES[0]));
		break;
	case __NR_write:
		handle_random_event(tcp, true, tcp->u_arg[2], WRITE_ERROR_CODES,
				    sizeof(WRITE_ERROR_CODES) / sizeof(WRITE_ERROR_CODES[0]));
		break;
	case __NR_readv:
	case __NR_preadv:
	case __NR_preadv2:
		handle_random_event(tcp, false, get_length_from_iovec(tcp, 1, 2), READ_ERROR_CODES,
				    sizeof(READ_ERROR_CODES) / sizeof(READ_ERROR_CODES[0]));
		break;
	case __NR_writev:
	case __NR_pwritev:
	case __NR_pwritev2:
		handle_random_event(tcp, true, get_length_from_iovec(tcp, 1, 2), WRITE_ERROR_CODES,
				    sizeof(WRITE_ERROR_CODES) / sizeof(WRITE_ERROR_CODES[0]));
		break;
	case __NR_recvfrom:
		handle_random_event(tcp, false, tcp->u_arg[2], READ_ERROR_CODES,
				    sizeof(READ_ERROR_CODES) / sizeof(READ_ERROR_CODES[0]));
		break;
	case __NR_sendto:
		handle_random_event(tcp, true, tcp->u_arg[2], WRITE_ERROR_CODES,
				    sizeof(WRITE_ERROR_CODES) / sizeof(WRITE_ERROR_CODES[0]));
		break;
	case __NR_recvmsg:
		handle_random_event(tcp, false, get_length_from_msg(tcp, 1), READ_ERROR_CODES,
				    sizeof(READ_ERROR_CODES) / sizeof(READ_ERROR_CODES[0]));
		break;
	case __NR_sendmsg:
		handle_random_event(tcp, true, get_length_from_msg(tcp, 1), WRITE_ERROR_CODES,
				    sizeof(WRITE_ERROR_CODES) / sizeof(WRITE_ERROR_CODES[0]));
		break;
	case __NR_recvmmsg:
		handle_random_event(tcp, false, get_length_from_mmsg(tcp, 1, 2), READ_ERROR_CODES,
				    sizeof(READ_ERROR_CODES) / sizeof(READ_ERROR_CODES[0]));
		break;
	case __NR_sendmmsg:
		handle_random_event(tcp, true, get_length_from_mmsg(tcp, 1, 2), WRITE_ERROR_CODES,
				    sizeof(WRITE_ERROR_CODES) / sizeof(WRITE_ERROR_CODES[0]));
		break;
	default:
		break;
	}
}

void
dst_fuzz_syscall_exiting(struct tcb *tcp)
{
	// if (last_injected) {
	// 	last_injected = false;
	// 	tcp->u_error = EAGAIN;
	// 	arch_set_error(tcp);
	// }
}

void
dst_fuzz_syscall_exiting_finish(struct tcb *tcp)
{
	if (!is_dst_fuzz) {
		return;
	}

	tmp_offset = 0;
	if (stored_regs != NULL) {
		struct user_regs_struct regs;
		ptrace(PTRACE_GETREGS, tcp->pid, NULL, &regs);
		if (compare_and_warning_regs_differences(&regs, stored_regs) < 2) {
			/*
			* we set rax to orig_rax, because it is changed after last 
			* syscall, and the old syscall's number is stored in orig_rax.
			*/
			regs.rax = stored_regs->orig_rax;
			regs.rip = stored_regs->rip - 2;
			fprintf(stderr,
				"We want to call syscall %s again, so we set rax to %lld and rip to %lld\n",
				sysent0[regs.rax].sys_name, regs.rax, regs.rip);
			ptrace(PTRACE_SETREGS, tcp->pid, NULL, &regs);
		}
	}
}

uint64_t
get_length_from_iovec(struct tcb *tcp, int iov_pos, int iovcnt_pos)
{
	uint64_t length = 0;
	for (uint64_t i = 0; i < tcp->u_arg[iovcnt_pos]; i++) {
		struct iovec iov;
		/*
		 * we must copy the data to our space to use
		 * two copy methods: ptrace(PTRACE_PEEKDATA, ...), ____NR_process_vm_readv
		 */
		umove(tcp, (tcp->u_arg[iov_pos] + sizeof(struct iovec) * i), &iov);
		length += iov.iov_len;
	}
	return length;
}

uint64_t
get_length_from_msg(struct tcb *tcp, int msg_pos)
{
	uint64_t length = 0;
	struct msghdr msg;
	umove(tcp, tcp->u_arg[msg_pos], &msg);
	for (uint64_t i = 0; i < msg.msg_iovlen; i++) {
		const struct iovec iov;
		umove(tcp, (kernel_ulong_t)(msg.msg_iov + sizeof(struct iovec) * i), &iov);
		length += iov.iov_len;
	}
	return length;
}

uint64_t
get_length_from_mmsg(struct tcb *tcp, int msgvec_pos, int vlen_pos)
{
	uint64_t length = 0;
	for (uint64_t i = 0; i < tcp->u_arg[vlen_pos]; i++) {
		struct mmsghdr msgvec;
		umove(tcp, (tcp->u_arg[msgvec_pos] + sizeof(struct mmsghdr) * i), &msgvec);
		for (uint64_t j = 0; j < msgvec.msg_hdr.msg_iovlen; j++) {
			const struct iovec iov;
			umove(tcp,
			      (kernel_ulong_t)(msgvec.msg_hdr.msg_iov + sizeof(struct iovec) * i),
			      &iov);
			length += iov.iov_len;
		}
	}
	return length;
}

int
compare_and_warning_regs_differences(const struct user_regs_struct *const new,
				     const struct user_regs_struct *const old)
{
#define COMPARE_REG(X)                                                                             \
	if (new->X != old->X) {                                                                    \
		fprintf(stderr,                                                                    \
			#X                                                                         \
			" in new is %lld, in old is %lld, please care about the side effects!\n",  \
			new->X, old->X);                                                           \
		diff_count++;                                                                      \
	}
	int diff_count = 0;
	COMPARE_REG(r15);
	COMPARE_REG(r14);
	COMPARE_REG(r13);
	COMPARE_REG(r12);
	COMPARE_REG(rbp);
	COMPARE_REG(rbx);
	COMPARE_REG(r11);
	COMPARE_REG(r10);
	COMPARE_REG(r9);
	COMPARE_REG(r8);
	COMPARE_REG(rax);
	COMPARE_REG(rcx);
	COMPARE_REG(rdx);
	COMPARE_REG(rsi);
	COMPARE_REG(rdi);
	COMPARE_REG(orig_rax);
	COMPARE_REG(rip);
	COMPARE_REG(cs);
	COMPARE_REG(eflags);
	COMPARE_REG(rsp);
	COMPARE_REG(ss);
	COMPARE_REG(fs_base);
	COMPARE_REG(gs_base);
	COMPARE_REG(ds);
	COMPARE_REG(es);
	COMPARE_REG(fs);
	COMPARE_REG(gs);
	return diff_count;
}

int
get_file_path(struct tcb *tcp, int fd, char **buf)
{
	int bufsiz = PATH_MAX;

	*buf = (char *)calloc(sizeof(char), bufsiz);
	if (*buf == NULL) {
		perror("calloc: ");
		return -1;
	}
	char path[100];
	sprintf(path, "/proc/%d/fd/%d", tcp->pid, fd);
	ssize_t nbytes = readlink(path, *buf, bufsiz);
	if (nbytes == -1) {
		perror("readlink");
		return -1;
	}

	return 0;
}

void
init_dst_fuzz(void)
{
	if (getenv("DST_FUZZ") != NULL) {
		char *res_shm_fuzz_coverage_map = getenv(FUZZ_COVERAGE_MAP_ENV_ID);
		if (!res_shm_fuzz_coverage_map) {
			fprintf(stderr, "%s",
				"\033[33m[FUZZ PRINT] Can not Get Environment Variable\033[0m\n");

#ifdef ENABLE_KAFKA
			if (getenv("USE_KAFKA") != NULL) {
				init_kafka_producer();
				use_kafka = true;
			}
#endif
		} else {
			fprintf(stderr, "\033[33m[FUZZ PRINT]Get Environment Variable %s\033[0m\n",
				res_shm_fuzz_coverage_map);
			fuzz_coverage_map =
				(uint8_t *)shmat((int)atoi(res_shm_fuzz_coverage_map), NULL, 0);
		}
	} else {
		fuzz_coverage_map = (uint8_t *)malloc(FUZZ_COVERAGE_MAP_SIZE * sizeof(uint8_t));
	}

	pathname = strace_malloc(strlen(params_for_tracee.argv[0]) + 10);
	if (params_for_tracee.argv[0][0] == '.') {
		if (params_for_tracee.argv[0][1] == '.') {
			strcpy(pathname, params_for_tracee.argv[0] + 3);
		} else {
			strcpy(pathname, params_for_tracee.argv[0] + 2);
		}
	} else {
		strcpy(pathname, params_for_tracee.argv[0]);
	}

	is_dst_fuzz = true;
}

#ifdef ENABLE_KAFKA
void
init_kafka_producer(void)
{
	rd_kafka_conf_t *conf = rd_kafka_conf_new(); // TODO: this leaked, but I don't care
	rd_kafka_t *rk = NULL; // TODO: this leaked, but I don't care
	char hostname[128];
	char errstr[512];
	if (gethostname(hostname, sizeof(hostname))) {
		fprintf(stderr, "%% Failed to lookup hostname\n");
		exit(1);
	}

	if (rd_kafka_conf_set(conf, "client.id", hostname, errstr, sizeof(errstr)) !=
	    RD_KAFKA_CONF_OK) {
		fprintf(stderr, "%% %s\n", errstr);
		exit(1);
	}
	if (rd_kafka_conf_set(conf, "bootstrap.servers", KAFKA_BOOTSTRAP_SERVER, errstr,
			      sizeof(errstr)) != RD_KAFKA_CONF_OK) {
		fprintf(stderr, "%% %s\n", errstr);
		exit(1);
	}
	if (!(rk = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr)))) {
		fprintf(stderr, "%% Failed to create new producer: %s\n", errstr);
		exit(1);
	}
	rd_kafka_topic_conf_t *topic_conf = rd_kafka_topic_conf_new();
	if (rd_kafka_topic_conf_set(topic_conf, "acks", "0", errstr, sizeof(errstr)) !=
	    RD_KAFKA_CONF_OK) {
		fprintf(stderr, "%% %s\n", errstr);
		exit(1);
	}
	topic_coverage = rd_kafka_topic_new(rk, KAFKA_COVERAGE_TOPIC, topic_conf);
}
#endif