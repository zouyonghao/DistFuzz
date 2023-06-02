/* -*- Mode: C; tab-width: 8; c-basic-offset: 2; indent-tabs-mode: nil; -*- */

#include "util.h"

int main(void) {
  char* tty = ttyname(STDIN_FILENO);
  atomic_printf("ttyname = %s\n", tty);
  if (!tty) {
    atomic_puts("No tty attached to stdin");
    fputs("EXIT-SUCCESS", stderr);
    return 0;
  }
  int fd = open(tty, O_RDWR);
  if (fd < 0 && errno == EACCES) {
    atomic_puts("tty access denied (su in progress?), skipping test");
    fputs("EXIT-SUCCESS", stderr);
    return 0;
  }
  test_assert(fd >= 0);
  test_assert(13 == write(fd, "EXIT-SUCCESS\n", 13));
  return 0;
}
