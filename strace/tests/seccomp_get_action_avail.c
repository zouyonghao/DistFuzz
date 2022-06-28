/*
 * Check decoding of seccomp SECCOMP_GET_ACTION_AVAIL.
 *
 * Copyright (c) 2017-2021 Dmitry V. Levin <ldv@strace.io>
 * All rights reserved.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "tests.h"
#include "scno.h"

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include <linux/seccomp.h>

static const char *errstr;

static long
k_seccomp(const kernel_ulong_t op, const kernel_ulong_t flags,
	  const kernel_ulong_t args)
{
	const long rc = syscall(__NR_seccomp, op, flags, args);
	errstr = sprintrc(rc);
	return rc;
}

int
main(void)
{
	TAIL_ALLOC_OBJECT_CONST_PTR(uint32_t, act);
	kernel_ulong_t op = (kernel_ulong_t) 0xfacefeed00000000ULL
				| SECCOMP_GET_ACTION_AVAIL;
	kernel_ulong_t flags = (kernel_ulong_t) 0xdeadbeef00000000ULL;

	struct {
		uint32_t val;
		const char *str;
	} actions [] = {
		{ 0, "SECCOMP_RET_KILL_THREAD" },
		{ ARG_STR(SECCOMP_RET_KILL_PROCESS) },
		{ ARG_STR(SECCOMP_RET_TRAP) },
		{ ARG_STR(SECCOMP_RET_ERRNO) },
		{ ARG_STR(SECCOMP_RET_USER_NOTIF) },
		{ ARG_STR(SECCOMP_RET_TRACE) },
		{ ARG_STR(SECCOMP_RET_LOG) },
		{ ARG_STR(SECCOMP_RET_ALLOW) },
		{ 0xffffffff, "0xffffffff /* SECCOMP_RET_??? */" }
	};

	for (unsigned int i = 0; i < ARRAY_SIZE(actions); ++i) {
		*act = actions[i].val;
		k_seccomp(op, flags, (uintptr_t) act);
		printf("seccomp(SECCOMP_GET_ACTION_AVAIL, 0, [%s]) = %s\n",
		       actions[i].str, errstr);
	}

	*act = actions[0].val;

	k_seccomp(op, flags, (uintptr_t) (act + 1));
	printf("seccomp(SECCOMP_GET_ACTION_AVAIL, 0, %p) = %s\n",
	       act + 1, errstr);

	if (F8ILL_KULONG_SUPPORTED) {
		k_seccomp(op, flags, f8ill_ptr_to_kulong(act));
		printf("seccomp(SECCOMP_GET_ACTION_AVAIL, 0, %#jx) = %s\n",
		       (uintmax_t) f8ill_ptr_to_kulong(act), errstr);
	}

	flags |= 0xcafef00d;
	k_seccomp(op, flags, 0);
	printf("seccomp(SECCOMP_GET_ACTION_AVAIL, %#x, NULL) = %s\n",
	       (unsigned int) flags, errstr);

	puts("+++ exited with 0 +++");
	return 0;
}
