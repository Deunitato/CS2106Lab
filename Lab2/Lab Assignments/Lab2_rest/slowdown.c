#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>


int childExit = 0;
void handler() {
   printf("Child stop at %d",childExit);
   childExit++;

}

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

  // End debug prints
  // Your code here
  /* Charlotte's algo planning (slowDown)
   1. If run time > tickinterval , kill the process
   2. For every w user ticks, my process can run one millsec -> sleep the process
   Ratio is w: ms
   Put the parent to sleep
   3. If process finish, the nothing..
   4. spawn a child to do it.. parent kill the child
   killing: SIGTSTP
   Continue: kill(yourChildPID, SIGCONT); 
   Signal library: https://pubs.opengroup.org/onlinepubs/009695399/basedefs/signal.h.html
  */
   
   int pauseTime = tick_interval*(slow_factor-1);
   //char* leftArgs[] = {argv[0], argv[optind+1], NULL};
   int i = 0;
   int nums = argc;

   char *leftArgs[100];
  for (int index = optind; index < nums; index++){ //copies args
    leftArgs[i] = argv[index];
    i++;
  }
  leftArgs[i] = NULL;
   char *progName = argv[optind];


 
    
   //do child here
  
   //signal(SIGCHLD,handler); //sets the handler to stop
   pid_t childPID = fork(); //create child
   
   if (childPID == 0 ) //if this is child
    { 
      signal(SIGSTOP,SIG_DFL);
      signal(SIGCONT,SIG_DFL);
      execv(progName, leftArgs);
      printf("Failed to run file");
    }
    printf("%d\n", childPID);
    
    while(1){
        usleep(tick_interval);
        //printf("%d is stopped after %d\n", childPID,tick_interval);
        kill(childPID, SIGSTOP);
        usleep(pauseTime);
        kill(childPID, SIGCONT);
        //printf("%d is started after %d\n", childPID,pauseTime);
    }



  return 0;
usage:
  fprintf(stderr, "Usage: %s [-s W] [-t T] executable args...\n", argv[0]);
  return 1;
}
