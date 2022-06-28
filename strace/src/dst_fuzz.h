#ifndef __DST_FUZZ_H_
#define __DST_FUZZ_H_

#include "defs.h"
#include "kernel_types.h"

extern bool is_dst_fuzz;

extern kernel_ulong_t stored_rip;

enum INJECT_EVENT { DELAY, FAIL, LOST, DUP };

extern void dst_fuzz_syscall_enter(struct tcb *);

extern void dst_fuzz_syscall_exiting(struct tcb *);

extern void dst_fuzz_syscall_exiting_finish(struct tcb *);

#endif // __DST_FUZZ_H_
