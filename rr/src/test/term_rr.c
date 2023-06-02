/* -*- Mode: C; tab-width: 8; c-basic-offset: 2; indent-tabs-mode: nil; -*- */

#include "util.h"

int main(void) {
  signal(SIGTERM, SIG_IGN);
  kill(getppid(), SIGTERM);
  atomic_puts("EXIT-SUCCESS");
  sleep(10000);
  return 0;
}
