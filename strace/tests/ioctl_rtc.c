/*
 * Check decoding of RTC ioctl commands.
 *
 * Copyright (c) 2016 Dmitry V. Levin <ldv@strace.io>
 * Copyright (c) 2016-2021 The strace developers.
 * All rights reserved.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/rtc.h>

static const unsigned long lmagic = (unsigned long) 0xdeadbeefbadc0dedULL;

static const char *errstr;

static int
do_ioctl(kernel_ulong_t cmd, kernel_ulong_t arg)
{
	int rc = ioctl(-1, cmd, arg);
	errstr = sprintrc(rc);

#ifdef INJECT_RETVAL
	if (rc != INJECT_RETVAL)
		error_msg_and_fail("Return value [%d] does not match"
				   " expectations [%d]", rc, INJECT_RETVAL);

	static char inj_errstr[4096];

	snprintf(inj_errstr, sizeof(inj_errstr), "%s (INJECTED)", errstr);
	errstr = inj_errstr;
#endif

	return rc;
}

static int
do_ioctl_ptr(kernel_ulong_t cmd, const void *arg)
{
	return do_ioctl(cmd, (uintptr_t) arg);
}

#ifdef INJECT_RETVAL
static void
skip_ioctls(int argc, const char *argv[])
{
	if (argc < 2)
		error_msg_and_fail("Usage: %s NUM_SKIP", argv[0]);

	unsigned long num_skip = strtoul(argv[1], NULL, 0);

	for (size_t i = 0; i < num_skip; ++i) {
		int rc = ioctl(-1, RTC_AIE_OFF, 0);
		printf("ioctl(-1, RTC_AIE_OFF) = %s%s\n", sprintrc(rc),
		       rc == INJECT_RETVAL ? " (INJECTED)" : "");
		if (rc == INJECT_RETVAL)
			return;
	}

	error_msg_and_fail("Issued %lu ioctl syscalls but failed"
			   " to detect an injected return code %d",
			   num_skip, INJECT_RETVAL);
}
#endif /* INJECT_RETVAL */

static void
print_rtc_time(const struct rtc_time *rt)
{
	printf("{tm_sec=%d, tm_min=%d, tm_hour=%d"
	       ", tm_mday=%d, tm_mon=%d, tm_year=%d",
	       rt->tm_sec, rt->tm_min, rt->tm_hour,
	       rt->tm_mday, rt->tm_mon, rt->tm_year);
#if VERBOSE
	printf(", tm_wday=%d, tm_yday=%d, tm_isdst=%d}",
	       rt->tm_wday, rt->tm_yday, rt->tm_isdst);
#else
	printf(", ...}");
#endif
}

int
main(int argc, const char *argv[])
{
#ifdef INJECT_RETVAL
	skip_ioctls(argc, argv);
#endif

	static const struct {
		unsigned int cmd;
		const char *str;
	} noarg_cmds[] = {
		{ ARG_STR(RTC_AIE_OFF) },
#ifdef HPPA
		{ RTC_AIE_ON, "PA_PERF_ON or RTC_AIE_ON" },
#else
		{ ARG_STR(RTC_AIE_ON) },
#endif
		{ ARG_STR(RTC_PIE_OFF) },
		{ ARG_STR(RTC_PIE_ON) },
		{ RTC_UIE_OFF, "PHN_NOT_OH or RTC_UIE_OFF" },
		{ ARG_STR(RTC_UIE_ON) },
#ifdef RTC_VL_CLR
		{ ARG_STR(RTC_VL_CLR), },
#endif
		{ ARG_STR(RTC_WIE_OFF) },
		{ ARG_STR(RTC_WIE_ON) },
	}, long_cmds[] = {
		{ ARG_STR(RTC_EPOCH_SET) },
		{ ARG_STR(RTC_IRQP_SET) },
	}, plong_cmds[] = {
		{ ARG_STR(RTC_EPOCH_READ) },
		{ ARG_STR(RTC_IRQP_READ) },
	}, ptr_cmds[] = {
		{ ARG_STR(RTC_ALM_READ) },
		{ ARG_STR(RTC_ALM_SET) },
		{ ARG_STR(RTC_EPOCH_READ) },
		{ ARG_STR(RTC_IRQP_READ) },
		{ ARG_STR(RTC_PLL_GET) },
		{ ARG_STR(RTC_PLL_SET) },
		{ ARG_STR(RTC_RD_TIME) },
		{ ARG_STR(RTC_SET_TIME) },
#ifdef RTC_VL_READ
		{ ARG_STR(RTC_VL_READ) },
#endif
		{ ARG_STR(RTC_WKALM_RD) },
		{ ARG_STR(RTC_WKALM_SET) },
	}, r_time_cmds[] = {
		{ ARG_STR(RTC_ALM_READ) },
		{ ARG_STR(RTC_RD_TIME) },
	}, w_time_cmds[] = {
		{ ARG_STR(RTC_ALM_SET) },
		{ ARG_STR(RTC_SET_TIME) },
	}, r_wkalrm_cmds[] = {
		{ ARG_STR(RTC_WKALM_RD) },
	}, w_wkalrm_cmds[] = {
		{ ARG_STR(RTC_WKALM_SET) },
	}, r_pll_cmds[] = {
		{ ARG_STR(RTC_PLL_GET) },
	}, w_pll_cmds[] = {
		{ ARG_STR(RTC_PLL_SET) },
	};

	for (size_t i = 0; i < ARRAY_SIZE(noarg_cmds); ++i) {
		do_ioctl(noarg_cmds[i].cmd, lmagic);
		printf("ioctl(-1, %s) = %s\n",
		       noarg_cmds[i].str, errstr);
	}

	for (size_t i = 0; i < ARRAY_SIZE(long_cmds); ++i) {
		do_ioctl(long_cmds[i].cmd, lmagic);
		printf("ioctl(-1, %s, %lu) = %s\n",
		       long_cmds[i].str, lmagic, errstr);
	}

	TAIL_ALLOC_OBJECT_CONST_PTR(unsigned long, plong);
	*plong = lmagic;

	for (size_t i = 0; i < ARRAY_SIZE(plong_cmds); ++i) {
		if (do_ioctl_ptr(plong_cmds[i].cmd, plong) < 0) {
			printf("ioctl(-1, %s, %p) = %s\n",
			       plong_cmds[i].str, plong, errstr);
		} else {
			printf("ioctl(-1, %s, [%lu]) = %s\n",
			       plong_cmds[i].str, *plong, errstr);
		}
	}

	void *const efault = tail_alloc(1);

	for (size_t i = 0; i < ARRAY_SIZE(ptr_cmds); ++i) {
		do_ioctl(ptr_cmds[i].cmd, 0);
		printf("ioctl(-1, %s, NULL) = %s\n",
		       ptr_cmds[i].str, errstr);
		do_ioctl_ptr(ptr_cmds[i].cmd, efault);
		printf("ioctl(-1, %s, %p) = %s\n",
		       ptr_cmds[i].str, efault, errstr);
	}

	TAIL_ALLOC_OBJECT_CONST_PTR(struct rtc_time, rt);
	fill_memory(rt, sizeof(*rt));

	for (size_t i = 0; i < ARRAY_SIZE(w_time_cmds); ++i) {
		do_ioctl_ptr(w_time_cmds[i].cmd, rt);
		printf("ioctl(-1, %s, ", w_time_cmds[i].str);
		print_rtc_time(rt);
		printf(") = %s\n", errstr);
	}

	for (size_t i = 0; i < ARRAY_SIZE(r_time_cmds); ++i) {
		if (do_ioctl_ptr(r_time_cmds[i].cmd, rt) < 0) {
			printf("ioctl(-1, %s, %p) = %s\n",
			       r_time_cmds[i].str, rt, errstr);
		} else {
			printf("ioctl(-1, %s, ", r_time_cmds[i].str);
			print_rtc_time(rt);
			printf(") = %s\n", errstr);
		}
	}

	TAIL_ALLOC_OBJECT_CONST_PTR(struct rtc_wkalrm, wk);
	fill_memory(wk, sizeof(*wk));

	for (size_t i = 0; i < ARRAY_SIZE(w_wkalrm_cmds); ++i) {
		do_ioctl_ptr(w_wkalrm_cmds[i].cmd, wk);
		printf("ioctl(-1, %s, {enabled=%hhu, pending=%hhu, time=",
		       w_wkalrm_cmds[i].str, wk->enabled, wk->pending);
		print_rtc_time(&wk->time);
		printf("}) = %s\n", errstr);
	}

	for (size_t i = 0; i < ARRAY_SIZE(r_wkalrm_cmds); ++i) {
		if (do_ioctl_ptr(r_wkalrm_cmds[i].cmd, wk) < 0) {
			printf("ioctl(-1, %s, %p) = %s\n",
			       r_wkalrm_cmds[i].str, wk, errstr);
		} else {
			printf("ioctl(-1, %s, {enabled=%hhu, pending=%hhu, time=",
			       r_wkalrm_cmds[i].str, wk->enabled, wk->pending);
			print_rtc_time(&wk->time);
			printf("}) = %s\n", errstr);
		}
	}

	TAIL_ALLOC_OBJECT_CONST_PTR(struct rtc_pll_info, pll);
	fill_memory(pll, sizeof(*pll));

	for (size_t i = 0; i < ARRAY_SIZE(w_pll_cmds); ++i) {
		do_ioctl_ptr(w_pll_cmds[i].cmd, pll);
		printf("ioctl(-1, %s, {pll_ctrl=%d, pll_value=%d"
		       ", pll_max=%d, pll_min=%d, pll_posmult=%d"
		       ", pll_negmult=%d, pll_clock=%ld}) = %s\n",
		       w_pll_cmds[i].str, pll->pll_ctrl, pll->pll_value,
		       pll->pll_max, pll->pll_min, pll->pll_posmult,
		       pll->pll_negmult, pll->pll_clock, errstr);
	}

	for (size_t i = 0; i < ARRAY_SIZE(r_pll_cmds); ++i) {
		if (do_ioctl_ptr(r_pll_cmds[i].cmd, pll) < 0) {
			printf("ioctl(-1, %s, %p) = %s\n",
			       r_pll_cmds[i].str, pll, errstr);
		} else {
			printf("ioctl(-1, %s, {pll_ctrl=%d, pll_value=%d"
			       ", pll_max=%d, pll_min=%d, pll_posmult=%d"
			       ", pll_negmult=%d, pll_clock=%ld}) = %s\n",
			       r_pll_cmds[i].str, pll->pll_ctrl, pll->pll_value,
			       pll->pll_max, pll->pll_min, pll->pll_posmult,
			       pll->pll_negmult, pll->pll_clock, errstr);
		}
	}

#ifdef RTC_VL_READ
	TAIL_ALLOC_OBJECT_CONST_PTR(unsigned int, pint);
	*pint = 1U << 4;

	if (do_ioctl_ptr(RTC_VL_READ, pint) < 0) {
		printf("ioctl(-1, RTC_VL_READ, %p) = %s\n", pint, errstr);
	} else {
		printf("ioctl(-1, RTC_VL_READ, [RTC_VL_BACKUP_SWITCH]) = %s\n",
		       errstr);
	}
#endif

	do_ioctl(_IO(0x70, 0x40), lmagic);
	printf("ioctl(-1, %s, %#lx) = %s\n", "NVRAM_INIT", lmagic, errstr);

	puts("+++ exited with 0 +++");
	return 0;
}
