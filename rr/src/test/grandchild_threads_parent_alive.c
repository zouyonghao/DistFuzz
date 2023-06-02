/* -*- Mode: C; tab-width: 8; c-basic-offset: 2; indent-tabs-mode: nil; -*- */

#include "util.h"

static void* start_thread(__attribute__((unused)) void* p) {
  sleep(1000);
  return NULL;
}

int main(void) {
  pid_t child;
  pthread_t thread;
  int pipe_fds[2];
  char ch;

  pipe(pipe_fds);
  child = fork();
  if (child > 0) {
    read(pipe_fds[0], &ch, 1);
    kill(child, 9);
    /* wait for the child to exit before we exit */
    waitpid(child, NULL, 0);
    return 0;
  }

  pthread_create(&thread, NULL, start_thread, NULL);
  atomic_puts("EXIT-SUCCESS");
  write(pipe_fds[1], &ch, 1);
  sleep(1000);
  return 0;
}
