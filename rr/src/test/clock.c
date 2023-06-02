/* -*- Mode: C; tab-width: 8; c-basic-offset: 2; indent-tabs-mode: nil; -*- */

#include "util.h"

static void breakpoint(void) {
  int break_here = 1;
  (void)break_here;
}

int main(void) {
  struct timespec ts;
  struct timeval tv;
  int i, err;

  err = clock_getres(CLOCK_MONOTONIC, &ts);
  test_assert(err == 0);
  atomic_printf("Clock resolution is >= %g us\n", ((double)ts.tv_nsec) / 1.0e3);

  memset(&ts, 0, sizeof(ts));
  memset(&tv, 0, sizeof(tv));

  breakpoint();
  for (i = 0; i < 100; ++i) {
    struct timespec ts_now;
    struct timeval tv_now;

    err = clock_gettime(CLOCK_MONOTONIC, &ts_now);
    test_assert(err == 0);
    test_assert(ts.tv_sec < ts_now.tv_sec ||
                (ts.tv_sec == ts_now.tv_sec && ts.tv_nsec <= ts_now.tv_nsec));
    ts = ts_now;

    if (i == 50) {
      breakpoint();
    }

    /* technically gettimeofday() isn't monotonic, but the
     * value of this check is higher than the remote
     * possibility of a spurious failure */
    err = gettimeofday(&tv_now, NULL);
    test_assert(err == 0);
    test_assert(tv.tv_sec < tv_now.tv_sec ||
                (tv.tv_sec == tv_now.tv_sec && tv.tv_usec <= tv_now.tv_usec));
    tv = tv_now;

    atomic_printf("cg: %g %llu, gtod: %g %llu\n", (double)ts.tv_sec,
                  (long long int)ts.tv_nsec, (double)tv.tv_sec,
                  (long long int)tv.tv_usec);
  }
  breakpoint();

  // Just make sure that rr has this system call. We don't actually try changing
  // CLOCK_REALTIME, just in case we're root and actually have permissions to do
  // that.
  err = clock_settime(CLOCK_MONOTONIC, &ts);
  test_assert(err == -1 && errno == EINVAL);

  atomic_puts("EXIT-SUCCESS");
  return 0;
}
