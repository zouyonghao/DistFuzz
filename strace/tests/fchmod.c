/*
 * Check decoding of fchmod syscall.
 *
 * Copyright (c) 2016 Fabien Siron <fabien.siron@epita.fr>
 * Copyright (c) 2016 Dmitry V. Levin <ldv@strace.io>
 * Copyright (c) 2016-2021 The strace developers.
 * All rights reserved.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "tests.h"
#include "scno.h"

#ifdef __NR_fchmod

# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>

# include "secontext.h"

int
main(void)
{
	/*
	 * Make sure the current workdir of the tracee
	 * is different from the current workdir of the tracer.
	 */
	create_and_enter_subdir("fchmod_subdir");

	char *my_secontext = SECONTEXT_PID_MY();

	static const char sample[] = "fchmod_sample_file";
	(void) unlink(sample);
	int fd = open(sample, O_CREAT|O_RDONLY, 0400);
	if (fd == -1)
		perror_msg_and_fail("open");

# ifdef YFLAG
	char *sample_realpath = get_fd_path(fd);
# endif

	const char *sample_secontext = SECONTEXT_FILE(sample);
	long rc = syscall(__NR_fchmod, fd, 0600);
# ifdef YFLAG
	printf("%s%s(%d<%s>%s, 0600) = %s\n",
# else
	printf("%s%s(%d%s, 0600) = %s\n",
# endif
	       my_secontext, "fchmod",
	       fd,
# ifdef YFLAG
	       sample_realpath,
# endif
	       sample_secontext,
	       sprintrc(rc));

	if (unlink(sample))
		perror_msg_and_fail("unlink");

	rc = syscall(__NR_fchmod, fd, 051);
# ifdef YFLAG
	printf("%s%s(%d<%s (deleted)>%s, 051) = %s\n",
# else
	printf("%s%s(%d%s, 051) = %s\n",
# endif
	       my_secontext, "fchmod",
	       fd,
# ifdef YFLAG
	       sample_realpath,
# endif
	       sample_secontext,
	       sprintrc(rc));

	rc = syscall(__NR_fchmod, fd, 004);
# ifdef YFLAG
	printf("%s%s(%d<%s (deleted)>%s, 004) = %s\n",
# else
	printf("%s%s(%d%s, 004) = %s\n",
# endif
	       my_secontext, "fchmod",
	       fd,
# ifdef YFLAG
	       sample_realpath,
# endif
	       sample_secontext,
	       sprintrc(rc));

	leave_and_remove_subdir();

	puts("+++ exited with 0 +++");
	return 0;
}

#else

SKIP_MAIN_UNDEFINED("__NR_fchmod")

#endif
