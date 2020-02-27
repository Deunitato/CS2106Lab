#define _GNU_SOURCE  
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

#include <sched.h>

typedef struct schedulee {
  char **argv;
} schedulee_t;
schedulee_t *make_schedulee(const char *);
void free_schedulee(schedulee_t *);


void deleteElement(pid_t arr[], int size, int c) 
{ 
  for(int i =c ; i< size-1; i++){
    arr[i] = arr[i+1];
  }
} 

int main(int argc, char *argv[]) {
  int exit_code = 0;
  int u_interval = 10000, n_cores = 1;
  char opt;
  while ((opt = getopt(argc, argv, "t:")) != -1) {
    switch (opt) {
    case 't':
      u_interval = atoi(optarg);
      break;
    default:
      fprintf(stderr, "Usage: %s [-t T] < config_file\n", argv[0]);
      return 1;
    }
  }

  size_t line_size = 0;
  char *line = NULL;
  size_t schedulees_size = 8;
  size_t schedulees_count = 0;
  schedulee_t **schedulees = calloc(schedulees_size, sizeof(schedulee_t *));

  if (getline(&line, &line_size, stdin) == -1 ||
      sscanf(line, "%d", &n_cores) != 1) {
        printf("THIS IS BAD! INVALID CONFIG");
        return 0;
  }

  while (getline(&line, &line_size, stdin) != -1) {
    int argv_offset = 0;
    sscanf(line, "%d", &argv_offset);
    char *schedulee_argv = line + argv_offset;
    schedulee_t *schedulee = make_schedulee(schedulee_argv);
    if (!schedulee) {
          printf("THIS IS BAD! FAILED2");
        return 0;
    }
    if (schedulees_count >= schedulees_size) {
      schedulees_size += 8;
      schedulees = realloc(schedulees, schedulees_size*sizeof(schedulee_t *));
      if (!schedulees) {
         printf("THIS IS BAD! FAILED3");
         return 0;
      }
    }
    schedulees[schedulees_count++] = schedulee;
  }


  //Setting up cores ===============================================

cpu_set_t set; 

//set for my scheduler
CPU_ZERO(&set); //clear the cores set
CPU_SET(0, &set); //set one
//set for scheduler only
/*if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
                   printf("sched_setaffinity");*/
//set for children
CPU_ZERO(&set); //clear the cores set
CPU_SET(1, &set); //set one

//============================================================================

//SET UP CHILD PROCESSES========================================
pid_t PIDarray[schedulees_count];
for(int i =0 ; i< schedulees_count; i ++){
  pid_t CHILDPID = fork(); //create that many child
  schedulee_t *schedulee = schedulees[i]; //seg
  if(CHILDPID==0){
    //usleep(i*1000);  /* wait 1ms for child 1, 2ms for child 2,..Inspired from: https://stackoverflow.com/questions/20449276/how-to-execute-child-processes-in-order-in-c. */ 
    raise(SIGSTOP); //suspend the process first
    execvp(schedulee->argv[0],schedulee->argv);
    printf("CHILD FAILED TO EXECUTE FILE");
  }
  else if(CHILDPID >0){ //parent
    PIDarray[i] = CHILDPID;
    if (sched_setaffinity(CHILDPID, sizeof(set), &set) == -1) //set my child to one core
                   printf("sched_setaffinity");
  }
  else{
    printf("Error creating child!");
    return 1;
  }
}
//=========================================

int progIndex = 0;
pid_t currentProg;
int numberOfProgs = schedulees_count;
currentProg = PIDarray[progIndex]; //points to first prog
int status;
while(1){
        if(waitpid(currentProg,&status,WNOHANG)==currentProg){ //it did terminate
         //remove the prog from my index
          deleteElement(PIDarray,numberOfProgs,progIndex);
          numberOfProgs--;
          if(numberOfProgs==0){break;} //there is no more programs :)
           // printf("\n%d Just ended\n", numberOfProgs);
            progIndex++; //go next
             if(progIndex>=numberOfProgs){progIndex=0;} //ensure doesnt go out of bound
             currentProg = PIDarray[progIndex];
        }

        
        kill(currentProg, SIGCONT); //continue
       // printf("I am running");
        usleep(u_interval); //i sleep
        kill(currentProg, SIGSTOP); //stop
       // printf("I am just stop");

        //move to next
        progIndex++; //go next
        if(progIndex==numberOfProgs){progIndex=0;} //ensure doesnt go out of bound
        currentProg = PIDarray[progIndex];
}
//==================================================================


  goto exit;
exit:
  if (schedulees) {
    for (size_t i = 0; i < schedulees_count; ++i) {
      free_schedulee(schedulees[i]);
    }
    free(schedulees);
  }
  if (line) {
    free(line);
  }
  return exit_code;
}

schedulee_t *make_schedulee(const char *argv) {
  schedulee_t *ret = calloc(1, sizeof(schedulee_t));
  char *ret_argv_buf = strdup(argv);
  size_t reg_argv_buf_index = 0;
  size_t ret_argv_nmemb = 8;
  size_t ret_argv_index = 0;
  ret->argv = calloc(ret_argv_nmemb, sizeof(char *));
  if (!ret || !ret_argv_buf || !ret->argv) {
    //goto fail;
    printf("THIS IS BAD! FAILED");
    return 0;
  }

  int last_was_tok = 0;
  while (1) {
    char *cur = ret_argv_buf + reg_argv_buf_index;
    if (*cur == '\0') {
      break;
    } else if (isspace(*cur)) {
      if (last_was_tok) {
        *cur = '\0';
      }
      last_was_tok = 0;
    } else {
      if (!last_was_tok) {
        if (ret_argv_index >= ret_argv_nmemb) {
          ret_argv_nmemb += 8;
          ret->argv = realloc(ret->argv, ret_argv_nmemb*sizeof(char *));
          if (!ret->argv) {
                printf("THIS IS BAD! FAILED1");
                 return 0;
          }
        }
        ret->argv[ret_argv_index++] = cur;
      }
      last_was_tok = 1;
    }
    reg_argv_buf_index++;
  }

  ret->argv[ret_argv_index] = NULL;

  return ret;

fail:
  if (ret) {
    if (ret->argv) {
      free(*ret->argv);
      free(ret->argv);
    }
    free(ret);
  }
  return NULL;
}

void free_schedulee(schedulee_t *schedulee) {
  if (schedulee) {
    if (schedulee->argv) {
      free(*schedulee->argv);
      free(schedulee->argv);
    }
    free(schedulee);
  }
}