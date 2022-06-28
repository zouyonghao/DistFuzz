/*
 * Copyright (c) 2015-2021 The strace developers.
 * All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

/* Return -1 on error or 1 on success (never 0!). */
static int
arch_get_syscall_args(struct tcb *tcp)
{
	static const unsigned int syscall_regs[MAX_ARGS] = { 4, 5, 6, 7, 0, 1 };

	for (unsigned int i = 0; i < n_args(tcp); ++i)
		tcp->u_arg[i] = sh_regs.regs[syscall_regs[i]];
	return 1;
}
