#include "Fuzzer.h"

#define FORSRV_FD	198
#define FORK_WAIT_MULT	10

static s32 fsrv_ctl_fd;
static s32 fsrv_st_fd;

void error_exit(string description) 
{
	cout << description << endl;
	exit(-1);
}

void init_forkserver(char **argv)
{
	static itimerval it;
	int st_pipe[2], ctl_pipe[2];
	int status;
	s32 rlen;

	cout << "Begin to set up forkserver" << endl;

	if (pipe(st_pipe) || pipe(ctl_pipe))
		error_exit("Fail to pipe() for st_pipe and ctl_pipe");
    
    string cmd;
	for (int i = 0; argv[i] != NULL; i++) {
		cmd = cmd + string(argv[i]) + " ";
	}
	cout << "cmd = " <<  cmd << endl;

	forkserver_pid = fork();
	if (forkserver_pid < 0) 
		error_exit("Fail to fork() for forkserver");

	if (!forkserver_pid) {
		// some resource limitation function can put here

		setsid();
		dup2(dev_null_fd, 0);
		dup2(dev_null_fd, 1);
		dup2(dev_null_fd, 2);

		if (dup2(ctl_pipe[0], FORSRV_FD) < 0) 
			error_exit("dup2() fail for ctl_pipe");
		if (dup2(st_pipe[1], FORSRV_FD + 1) < 0) 
			error_exit("dup2() fail for st_pipe");
		
		close(ctl_pipe[0]);
		close(ctl_pipe[1]);
		close(st_pipe[0]);
		close(st_pipe[1]);

		close(dev_null_fd);

		setenv("ASAN_OPTIONS",
					 "abort_on_error=1:"
					 "detect_leaks=0:"
					 "symbolize=0:"
					 "allocator_may_return_null=1",
					 0);

		// execv(target_path.c_str(), argv);
		system(cmd.c_str());
		exit(0);
	}

	close(ctl_pipe[0]);
	close(st_pipe[1]);

	fsrv_ctl_fd = ctl_pipe[1];
	fsrv_st_fd = st_pipe[0];

	// wait for forkserver to come up, but don't wait too long
	it.it_value.tv_sec = (exec_tmout * FORK_WAIT_MULT) / 1000;
	it.it_value.tv_usec = ((exec_tmout * FORK_WAIT_MULT) % 1000) * 1000;
	setitimer(ITIMER_REAL, &it, NULL);

	rlen = read(fsrv_st_fd, &status, 4);

	it.it_value.tv_sec = 0;
	it.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &it, NULL);

	if (rlen == 4) {
		cout << "All right - forkserver is up" << endl;
		return;
	}

	error_exit("Enter forkserver fail");
}

void call_forkserver(u32 &prev_time_out, s32 &child_pid)
{
	s32 res;
	if ((res = write(fsrv_ctl_fd, &prev_time_out, 4)) != 4)
		error_exit("Fail to write() prev_time_out in fsrv_ctl_fd");
	
	if ((res = read(fsrv_st_fd, &child_pid, 4)) != 4)
		error_exit("Fail to read() child_pid in fsrv_st_fd");
	
	if (child_pid <= 0)
		error_exit("Forkserver is misbehaving (OOM?)");
}

void get_child_status(int &status)
{
	s32 res;
	if ((res = read(fsrv_st_fd, &status, 4)) != 4)
		error_exit("Fail to read() status in fsrv_st_fd");
}