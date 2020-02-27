#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <wait.h>

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

  // If there are more non-option arguments
  if (optind >= argc) {
    goto usage;
  }

  // // Remove the following debug prints from your solution
  // printf("slow_factor=%d; tick_interval=%d\n",
  //        slow_factor, tick_interval);
  // printf("exec argv=[");
  // for (int i = optind; i < argc; ++i) {
  //   printf("\"%s\", ", argv[i]);
  // }
  // printf("\b\b]\n");
  // // End debug prints

  // // Your code here
  __pid_t pid, ppid, looper;
  ppid = getpid();

  //first child
  if((pid = fork()) == 0){
    execl(argv[optind], argv[optind], argv[optind + 1], NULL);
  }

  long tick_count = 1;
  void parent_handler(int sig){
    if(sig == SIGUSR1){
      if(tick_count % slow_factor == 0){
        kill(pid, SIGCONT);
      } else {
        kill(pid, SIGSTOP);
      }
      tick_count++;
    }
  }
  
  signal(SIGUSR1, parent_handler);
  //second child
  if((looper = fork()) == 0){
    int track_time = 1;
    void looper_handler(int sig){
      if(sig == SIGUSR2){
      track_time = 0;
      }
  }
    signal(SIGUSR2, looper_handler);
    while(track_time){
      kill(ppid, SIGUSR1);
      usleep(tick_interval);
    }
    return 0;
  }

  waitpid(pid, NULL, 0);
  kill(looper, SIGUSR2);
  waitpid(looper, NULL, 0);
  return 0;
usage:
  fprintf(stderr, "Usage: %s [-s W] [-t T] executable args...\n", argv[0]);
  return 1;
}
