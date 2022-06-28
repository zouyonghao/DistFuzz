/*
 * Copyright (c) 2015 Dmitry V. Levin <ldv@strace.io>
 * Copyright (c) 2015-2021 The strace developers.
 * All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "defs.h"
#include <linux/memfd.h>
#include "xlat/memfd_create_flags.h"

#ifndef MFD_NAME_MAX_LEN
# define MFD_NAME_MAX_LEN (255 - (sizeof("memfd:") - 1))
#endif

SYS_FUNC(memfd_create)
{
	/* name */
	printstr_ex(tcp, tcp->u_arg[0], MFD_NAME_MAX_LEN + 1,
		    QUOTE_0_TERMINATED);
	tprint_arg_next();

	unsigned int flags = tcp->u_arg[1];

	if (!flags || xlat_verbose(xlat_verbosity) != XLAT_STYLE_ABBREV)
		PRINT_VAL_X(flags);

	if (!flags || xlat_verbose(xlat_verbosity) == XLAT_STYLE_RAW)
		return RVAL_DECODED | RVAL_FD;

	if (xlat_verbose(xlat_verbosity) == XLAT_STYLE_VERBOSE)
		tprint_comment_begin();

	const unsigned int mask = MFD_HUGE_MASK << MFD_HUGE_SHIFT;
	const unsigned int hugetlb_value = flags & mask;
	flags &= ~mask;

	if (flags || !hugetlb_value)
		printflags_ex(flags, "MFD_???", XLAT_STYLE_ABBREV,
			      memfd_create_flags, NULL);

	if (hugetlb_value)
		tprintf("%s%u<<MFD_HUGE_SHIFT",
			flags ? "|" : "",
			hugetlb_value >> MFD_HUGE_SHIFT);

	if (xlat_verbose(xlat_verbosity) == XLAT_STYLE_VERBOSE)
		tprint_comment_end();

	return RVAL_DECODED | RVAL_FD;
}
