#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {
  int slow_factor = 1, tick_interval = 10000;
  char opt;
  while ((opt = getopt(argc, argv, "s:t:")) != -1) {
    switch (opt) {
    case 's':
      slow_factor = atoi(optarg);
      break;
    case 't':
      tick_interval = atoi(optarg);
      break;
    default:
      goto usage;
    }
  }

  if (optind >= argc) {
    goto usage;
  }

  // Remove the following debug prints from your solution
  printf("slow_factor=%d; tick_interval=%d\n",
         slow_factor, tick_interval);
  printf("exec argv=[");
  for (int i = optind; i < argc; ++i) {
    printf("\"%s\", ", argv[i]);
  }
  printf("\b\b]\n");
  // End debug prints
  // Your code here

  return 0;
usage:
  fprintf(stderr, "Usage: %s [-s W] [-t T] executable args...\n", argv[0]);
  return 1;
}
