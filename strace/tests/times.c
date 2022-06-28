/*
 * Check decoding of times syscall.
 *
 * Copyright (c) 2015-2021 Eugene Syromyatnikov <evgsyr@gmail.com>
 * Copyright (c) 2015-2016 Dmitry V. Levin <ldv@strace.io>
 * Copyright (c) 2015-2021 The strace developers.
 * All rights reserved.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

/**
 * @file
 * This test burns some CPU cycles in user space and kernel space in order to
 * get some non-zero values returned by times(2).
 */

#include "tests.h"
#include <fcntl.h>
#include <sched.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "scno.h"
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/wait.h>

enum {
	NUM_USER_ITERS_SQRT = 2000,
	NUM_USER_ITERS = NUM_USER_ITERS_SQRT * NUM_USER_ITERS_SQRT,
	READ_BUF_SIZE = 65536,
	READ_ITER = 128,
	PARENT_CPUTIME_LIMIT_NSEC = 300000000,
	CHILD_CPUTIME_LIMIT_NSEC = 500000000,
};

static uint64_t
nsecs(struct timespec *ts)
{
	return (uint64_t) ts->tv_sec * 1000000000 + ts->tv_nsec;
}

static void
enjoy_time(uint64_t cputime_limit)
{
	struct timespec ts = { 0 };
	volatile int dummy = 0;

	/* Enjoying my user time */
	for (size_t i = 0; i < NUM_USER_ITERS_SQRT; ++i) {
		if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts) == 0) {
			if (nsecs(&ts) >= cputime_limit)
				break;
		}

		for (size_t j = 0; j < NUM_USER_ITERS; ++j)
			++dummy;
	}

	/* Enjoying my system time */
	ssize_t ret;
	int fd;
	char buf[READ_BUF_SIZE];

	while (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts) == 0) {
		for (size_t i = 0; i < READ_ITER; i++) {
			/* We are fine even if the calls fail. */
			fd = open("/proc/self/status", O_RDONLY);
			/*
			 * Working around "ignoring return value of 'read'
			 * declared with attribute 'warn_unused_result'".
			 */
			ret = read(fd, buf, sizeof(buf));
			close(fd);
			if (ret)
				continue;
		}

		if (nsecs(&ts) >= cputime_limit * 3)
			break;

		sched_yield();
	}
}

int
main(void)
{
	enjoy_time(PARENT_CPUTIME_LIMIT_NSEC);

	pid_t pid = fork();
	if (pid < 0)
		perror_msg_and_fail("fork");

	if (pid == 0) {
		enjoy_time(CHILD_CPUTIME_LIMIT_NSEC);

		return 0;
	} else {
		wait(NULL);
	}

	struct tms tbuf;
	unsigned long long llres;

	/*
	 * On systems where user's and kernel's long types are the same,
	 * prefer direct times syscall over libc's times function because
	 * the latter is more prone to return value truncation.
	 */

#if defined __x86_64__ && defined __ILP32__
	register long arg asm("rdi") = (long) &tbuf;
	asm volatile("syscall\n\t"
		     : "=a"(llres)
		     : "0"(__NR_times), "r"(arg)
		     : "memory", "cc", "r11", "cx");
	if (llres > 0xfffffffffffff000)
		return 77;
#elif defined LINUX_MIPSN32
	clock_t res = times(&tbuf);

	if ((clock_t) -1 == res)
		perror_msg_and_skip("times");
	if (sizeof(res) < sizeof(unsigned long long))
		llres = (unsigned long) res;
	else
		llres = res;
#else
	long res = syscall(__NR_times, &tbuf);

	if (-1L == res)
		perror_msg_and_skip("times");
	else
		llres = (unsigned long) res;
#endif

	long clk_tck = sysconf(_SC_CLK_TCK);
	int precision = clk_tck > 100000000 ? 9
			: clk_tck > 10000000 ? 8
			: clk_tck > 1000000 ? 7
			: clk_tck > 100000 ? 6
			: clk_tck > 10000 ? 5
			: clk_tck > 1000 ? 4
			: clk_tck > 100 ? 3
			: clk_tck > 10 ? 2
			: clk_tck > 1 ? 1 : 0;

	if (!XLAT_RAW && clk_tck > 0) {
		printf("times({tms_utime=%llu /* %.*f s */"
		       ", tms_stime=%llu /* %.*f s */"
		       ", tms_cutime=%llu /* %.*f s */"
		       ", tms_cstime=%llu /* %.*f s */}) = %llu\n",
		       (unsigned long long) tbuf.tms_utime,
		       precision, (double) tbuf.tms_utime / clk_tck,
		       (unsigned long long) tbuf.tms_stime,
		       precision, (double) tbuf.tms_stime / clk_tck,
		       (unsigned long long) tbuf.tms_cutime,
		       precision, (double) tbuf.tms_cutime / clk_tck,
		       (unsigned long long) tbuf.tms_cstime,
		       precision, (double) tbuf.tms_cstime / clk_tck,
		       llres);
	} else {
		printf("times({tms_utime=%llu, tms_stime=%llu"
		       ", tms_cutime=%llu, tms_cstime=%llu}) = %llu\n",
		       (unsigned long long) tbuf.tms_utime,
		       (unsigned long long) tbuf.tms_stime,
		       (unsigned long long) tbuf.tms_cutime,
		       (unsigned long long) tbuf.tms_cstime,
		       llres);
	}

	puts("+++ exited with 0 +++");

	return 0;
}
