/*
 * Check decoding of get_robust_list and set_robust_list syscalls.
 *
 * Copyright (c) 2015-2016 Dmitry V. Levin <ldv@strace.io>
 * Copyright (c) 2015-2021 The strace developers.
 * All rights reserved.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "tests.h"
#include "scno.h"
#include "pidns.h"

#include <stdio.h>
#include <unistd.h>

static const char *
sprintaddr(void *addr)
{
	static char buf[sizeof(addr) * 2 + sizeof("0x")];

	if (!addr)
		return "NULL";
	else
		snprintf(buf, sizeof(buf), "%p", addr);

	return buf;
}

int
main(void)
{
	PIDNS_TEST_INIT;

	const pid_t pid = getpid();
	const char *pid_str = pidns_pid2str(PT_TGID);
	const long long_pid = (unsigned long) (0xdeadbeef00000000LL | pid);
	TAIL_ALLOC_OBJECT_CONST_PTR(void *, p_head);
	TAIL_ALLOC_OBJECT_CONST_PTR(size_t, p_len);

	/* It has dual-use as a marker of the beginning of the test output */
	long rc = syscall(__NR_get_robust_list, 0, 0, 0);
	pidns_print_leader();
	printf("get_robust_list(0, NULL, NULL) = %s\n", sprintrc(rc));

	if (syscall(__NR_get_robust_list, long_pid, p_head, p_len))
		perror_msg_and_skip("get_robust_list");
	pidns_print_leader();
	printf("get_robust_list(%d%s, [%s], [%lu]) = 0\n",
	       pid, pid_str, sprintaddr(*p_head),
	       (unsigned long) *p_len);

	void *head = tail_alloc(*p_len);
	if (syscall(__NR_set_robust_list, head, *p_len))
		perror_msg_and_skip("set_robust_list");
	pidns_print_leader();
	printf("set_robust_list(%p, %lu) = 0\n",
	       head, (unsigned long) *p_len);

	if (syscall(__NR_get_robust_list, long_pid, p_head, p_len))
		perror_msg_and_skip("get_robust_list");
	pidns_print_leader();
	printf("get_robust_list(%d%s, [%s], [%lu]) = 0\n",
	       pid, pid_str, sprintaddr(*p_head),
	       (unsigned long) *p_len);

	pidns_print_leader();
	puts("+++ exited with 0 +++");
	return 0;
}
