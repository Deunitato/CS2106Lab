#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>

#define NSECS_PER_SEC 1000000000;
#define errExit(msg)    \
  do                    \
  {                     \
    perror(msg);        \
    exit(EXIT_FAILURE); \
  } while (0)

void start_timer(timer_t *timerid, int sig, int usec)
{
  struct itimerspec its;
  int nanoseconds = usec * 1000;

  its.it_value.tv_sec = nanoseconds / NSECS_PER_SEC; //waits for 5 seconds before sending timer signal
  its.it_value.tv_nsec = nanoseconds % NSECS_PER_SEC;

  its.it_interval.tv_sec = nanoseconds / NSECS_PER_SEC; //sends timer signal every 5 seconds
  its.it_interval.tv_nsec = nanoseconds % NSECS_PER_SEC;;

  struct sigevent sev;
  sev.sigev_notify = SIGEV_SIGNAL;
  sev.sigev_signo = sig;
  sev.sigev_value.sival_ptr = timerid;

  if (timer_create(CLOCK_MONOTONIC, &sev, timerid) == -1)
  {
    errExit("timer_create");
  }

  if (timer_settime(*timerid, 0, &its, NULL) == -1)
  {
    errExit("timer_settime");
  }
}

void stop_timer(timer_t id)
{
  struct itimerspec its;

  its.it_value.tv_sec = 0;
  its.it_value.tv_nsec = 0;

  its.it_interval.tv_sec = 0;
  its.it_interval.tv_nsec = 0;

  timer_settime(id, 0, &its, NULL);
}

int main(int argc, char *argv[])
{
  int slow_factor = 1, tick_interval = 10000;
  char opt;
  while ((opt = getopt(argc, argv, "s:t:")) != -1)
  {
    switch (opt)
    {
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
  // printf("slow_factor=%d; tick_interval=%d\n",
  //        slow_factor, tick_interval);
  // printf("optind is: %d\n", optind);
  // printf("argc: %d\n", argc);
  // printf("exec argv=[");
  // for (int i = optind; i < argc; ++i)
  // {
  //   printf("\"%s\", ", argv[i]);
  // }
  // printf("\b\b]\n");
  // End debug prints

  // Your code here
  pid_t pid;
  timer_t timerid;
  long tick_num = 1;
  int prev_sig = SIGCONT;

  void tick(int sig)
  {
    if (tick_num % slow_factor == 0) {
      if (prev_sig == SIGSTOP) {
        kill(pid, SIGCONT);
        prev_sig = SIGCONT;
      }
    } else {
      if (prev_sig == SIGCONT) {
        kill(pid, SIGSTOP);
        prev_sig = SIGSTOP;
      }
    }
    tick_num++;
  }

  if ((pid = fork()) == 0) {
    char *command = argv[optind];
    char *param[argc - optind + 1];
    memcpy( param, argv + optind, sizeof(argv[0]) * (argc - optind + 1)) ;
    execvp( command, param );
  } else  {
    signal(SIGUSR1, tick);
    start_timer( &timerid, SIGUSR1, tick_interval );
    wait(NULL);
    return 0;
  }

  usage:
    fprintf(stderr, "Usage: %s [-s W] [-t T] executable args...\n", argv[0]);
    return 1;
}