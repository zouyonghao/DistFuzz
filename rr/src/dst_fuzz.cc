#include <asm/unistd_64.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/socket.h>

#include "dst_fuzz.h"
#include "dst_random.h"
#include "utils/dst_share_mem_util.h"

#include "log.h"

using namespace rr;

int READ_ERROR_CODES[] = { 0,      EAGAIN, EWOULDBLOCK, EBADF,
                           EFAULT, EINTR,  EINVAL,      EIO };
int WRITE_ERROR_CODES[] = { EAGAIN, EWOULDBLOCK, EBADF, EDESTADDRREQ, EDQUOT,
                            EFAULT, EFBIG,       EINTR, EINVAL,       EIO,
                            ENOSPC, EPERM,       EPIPE };

int RECV_ERROR_CODES[] = {
  EAGAIN, EWOULDBLOCK, EBADF,  ECONNREFUSED, EFAULT,
  EINTR,  EINVAL,      ENOMEM, ENOTCONN,     ENOTSOCK
};
int SEND_ERROR_CODES[] = { EACCES,     EAGAIN,       EALREADY, EBADF,
                           ECONNRESET, EDESTADDRREQ, EFAULT,   EINTR,
                           EINVAL,     EISCONN,      EMSGSIZE, ENOBUFS,
                           ENOMEM,     ENOTCONN,     ENOTSOCK, EOPNOTSUPP,
                           EPIPE };

bool dst_fuzz_inited = false;
static uint8_t* fuzz_coverage_map;
#define FUZZ_COVERAGE_MAP_ENV_ID "AFLCplusplus_BRANCH_TRACE_SHM_ID"
#define FUZZ_COVERAGE_MAP_SIZE (1u << 23)

static bool printed_fuzzing_stopped = false;
static int ignore_range = 1;
static uint64_t event_index = 0;
uint64_t get_hash_value(bool syscall_no, uint64_t position, size_t length,
                        bool fault_injected) {
  // int node_id = getenv("NODE_ID") != NULL ? atoi(getenv("NODE_ID")) : 0;
  int node_id = 0;
  // uint64_t hash_value =
  //     ((event_index + is_send * 10 + length + node_id * 100 + position)
  //      << fault_injected) %
  //     FUZZ_COVERAGE_MAP_SIZE;
  uint64_t hash_value =
      ((event_index + syscall_no * 10 + length + node_id * 100 + position)
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

int get_file_path(RecordTask* t, int fd, char** buf) {
  int bufsiz = PATH_MAX;

  *buf = (char*)calloc(sizeof(char), bufsiz);
  if (*buf == NULL) {
    perror("calloc: ");
    return -1;
  }
  char path[100];
  sprintf(path, "/proc/%d/fd/%d", t->rec_tid, fd);
  ssize_t nbytes = readlink(path, *buf, bufsiz);
  if (nbytes == -1) {
    perror("readlink");
    return -1;
  }

  return 0;
}

void handle_random_event(RecordTask* t, bool is_send, size_t length,
                         int error_codes[], int error_codes_length) {

  /* If fuzzing is disabled, we call the original function. */
  if (!get_is_fuzzing()) {
    if (!printed_fuzzing_stopped) {
      fprintf(stderr,
              "rr: fuzzing is stopped, will use kernel_func directly.\n");
      printed_fuzzing_stopped = true;
    }
    return;
  }

  int fd = t->regs().arg1_signed();

  /* Ignore stdin, stdout, stderr */
  if (fd < 3) {
    return;
  }

  char* file_path;
  if (get_file_path(t, fd, &file_path) == -1) {
    perror("get_file_path:");
    return;
  }

  // printf("%s\n", file_path);

  // bool is_file = false;
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
      const char* no_fault_files_env = getenv("NO_FAULT_FILES");
      if (no_fault_files_env != NULL && strlen(no_fault_files_env) > 0) {
        int files_length = strlen(no_fault_files_env);
        char* no_fault_files = (char*)malloc(files_length + 1);
        strcpy(no_fault_files, no_fault_files_env);
        fprintf(stderr, "no_fault_files is %s\n", no_fault_files);
        const char* delim = ",";
        char* no_fault_file = strtok(no_fault_files, delim);
        while (no_fault_file != NULL) {
          if (strstr(file_path, no_fault_file) != NULL) {
            fprintf(stderr, "%s is skipped\n", no_fault_file);
            return;
          }
          no_fault_file = strtok(NULL, delim);
        }
        free(no_fault_files);
      }

      break;
  }

  bool fault_injected = false;
  uint8_t random;
  Registers r = t->regs();
  long ret_val = 0;

  if (getenv("NO_FAULT") != NULL) {
    goto EXIT;
  }

  random = __dst_get_random_uint8_t();
  // fprintf(stderr, "random is %u\n", random);
  if (random < 50) {
    goto EXIT;
  }

  fault_injected = true;
  fprintf(stderr, "get file path %s\n", file_path);

  switch (__dst_get_random_uint8_t() % 2) {
    case 0:
      // delay
      usleep(__dst_get_random_uint16_t() * 1e6);
      fprintf(stderr, "inject delay to process %d syscall %ld\n", t->tid,
              t->regs().original_syscallno());

      break;
    case 1:
      // fail
      if (is_eventfd) {
        ret_val = EAGAIN;
      } else {
        ret_val = error_codes[__dst_get_random_uint8_t() % error_codes_length];
      }
      r.set_syscall_result(-ret_val);
      t->set_regs(r);
      fprintf(stderr, "inject fail to process %d syscall %ld, is_send? %d\n",
              t->tid, t->regs().original_syscallno(), is_send);
      break;
      // TODO: lost
      // TODO: dup
    default:
      break;
  }

EXIT:
  // uint64_t hash_index = get_hash_value(is_send, tmp_offset, length,
  // fault_injected);
  uint64_t hash_index =
      get_hash_value(t->regs().original_syscallno(), 0, length, fault_injected);
  fuzz_coverage_map[hash_index]++;
}

uint64_t get_length_from_iovec(RecordTask* t, uintptr_t iov_ptr, int iovcnt) {
  uint64_t length = 0;
  auto iovs = t->read_mem<struct iovec>(iov_ptr, iovcnt);
  for (auto iov : iovs) {
    length += iov.iov_len;
  }
  return length;
}

uint64_t get_length_from_msg(RecordTask* t, uintptr_t msghdr_ptr) {
  uint64_t length = 0;
  auto msg = t->read_mem<struct msghdr>(msghdr_ptr);
  auto iovs = t->read_mem<struct iovec>((uintptr_t)msg.msg_iov, msg.msg_iovlen);
  for (auto iov : iovs) {
    length += iov.iov_len;
  }
  return length;
}

uint64_t get_length_from_mmsg(RecordTask* t, uintptr_t mmsghdr_ptr, int vlen) {
  uint64_t length = 0;
  auto mmsghdrs = t->read_mem<struct mmsghdr>(mmsghdr_ptr, vlen);
  for (auto msghdr : mmsghdrs) {
    length += msghdr.msg_len;
  }
  return length;
}

void dst_fuzz(RecordTask* t) {
  if (!dst_fuzz_inited) {
    if (getenv("DST_FUZZ") != NULL) {
      char* res_shm_fuzz_coverage_map = getenv(FUZZ_COVERAGE_MAP_ENV_ID);
      if (!res_shm_fuzz_coverage_map) {
        printf(
            "\033[33m[FUZZ PRINT] Can not Get Environment Variable\033[0m\n");
      } else {
        printf("\033[33m[FUZZ PRINT]Get Environment Variable %s\033[0m\n",
               res_shm_fuzz_coverage_map);
        fuzz_coverage_map =
            (uint8_t*)shmat((int)atoi(res_shm_fuzz_coverage_map), NULL, 0);
      }
    } else {
      fuzz_coverage_map =
          (uint8_t*)malloc(FUZZ_COVERAGE_MAP_SIZE * sizeof(uint8_t));
    }
    dst_fuzz_inited = true;
  }

  switch (t->regs().original_syscallno()) {
    case __NR_read:
      handle_random_event(t, false, t->regs().arg3_signed(), READ_ERROR_CODES,
                          sizeof(READ_ERROR_CODES) /
                              sizeof(READ_ERROR_CODES[0]));
      break;
    case __NR_write:
      handle_random_event(t, true, t->regs().arg3_signed(), WRITE_ERROR_CODES,
                          sizeof(WRITE_ERROR_CODES) /
                              sizeof(WRITE_ERROR_CODES[0]));
      break;
    case __NR_readv:
    case __NR_preadv:
    case __NR_preadv2:
      handle_random_event(
          t, false,
          get_length_from_iovec(t, t->regs().arg2(), t->regs().arg3_signed()),
          READ_ERROR_CODES,
          sizeof(READ_ERROR_CODES) / sizeof(READ_ERROR_CODES[0]));
      break;
    case __NR_writev:
    case __NR_pwritev:
    case __NR_pwritev2:
      handle_random_event(
          t, true,
          get_length_from_iovec(t, t->regs().arg2(), t->regs().arg3_signed()),
          WRITE_ERROR_CODES,
          sizeof(WRITE_ERROR_CODES) / sizeof(WRITE_ERROR_CODES[0]));
      break;
    case __NR_recvfrom:
      handle_random_event(t, false, t->regs().arg3(), READ_ERROR_CODES,
                          sizeof(READ_ERROR_CODES) /
                              sizeof(READ_ERROR_CODES[0]));
      break;
    case __NR_sendto:
      handle_random_event(t, true, t->regs().arg3(), WRITE_ERROR_CODES,
                          sizeof(WRITE_ERROR_CODES) /
                              sizeof(WRITE_ERROR_CODES[0]));
      break;
    case __NR_recvmsg:
      handle_random_event(
          t, false, get_length_from_msg(t, t->regs().arg2()), READ_ERROR_CODES,
          sizeof(READ_ERROR_CODES) / sizeof(READ_ERROR_CODES[0]));
      break;
    case __NR_sendmsg:
      handle_random_event(
          t, true, get_length_from_msg(t, t->regs().arg2()), WRITE_ERROR_CODES,
          sizeof(WRITE_ERROR_CODES) / sizeof(WRITE_ERROR_CODES[0]));
      break;
    case __NR_recvmmsg:
      handle_random_event(
          t, false, get_length_from_mmsg(t, t->regs().arg2(), t->regs().arg3()),
          READ_ERROR_CODES,
          sizeof(READ_ERROR_CODES) / sizeof(READ_ERROR_CODES[0]));
      break;
    case __NR_sendmmsg:
      handle_random_event(
          t, true, get_length_from_mmsg(t, t->regs().arg2(), t->regs().arg3()),
          WRITE_ERROR_CODES,
          sizeof(WRITE_ERROR_CODES) / sizeof(WRITE_ERROR_CODES[0]));
      break;
    default:
      break;
  }

  fuzz_coverage_map[0] = 0;
}