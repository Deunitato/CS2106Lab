#define _GNU_SOURCE

#include <ctype.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct schedulee {
    char **argv;
} schedulee_t;
schedulee_t *make_schedulee(const char *);
void free_schedulee(schedulee_t *);

int main(int argc, char *argv[])
{
    int tick_interval = 10000, n_cores = 1;
    char opt;

    while ((opt = getopt(argc, argv, "t:")) != -1) {
        switch (opt) {
        case 't':
            tick_interval = atoi(optarg);
            break;
        default:
            fprintf(stderr, "Usage: %s [-t T] executable args...\n", argv[0]);
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
        fprintf(stderr, "Invalid configuration file\n");
        // should free all
        return 1;
    }

    while (getline(&line, &line_size, stdin) != -1) {
        schedulee_t *schedulee = make_schedulee(line);
        if (!schedulee) {
            // should free all
            return 1;
        }
        if (schedulees_count >= schedulees_size) {
            schedulees_size += 8;
            schedulees = realloc(schedulees, schedulees_size * sizeof(schedulee_t *));
            if (!schedulees) {
                // should free all
                return 1;
            }
        }
        schedulees[schedulees_count++] = schedulee;
    }

    // Remove the following debug prints from your solution
    printf("tick_interval= %d\n", tick_interval);
    for (size_t i = 0; i < schedulees_count; ++i) {
        schedulee_t *schedulee = schedulees[i];
        printf("schedulee %zu: argv: [", i);
        for (size_t j = 0; schedulee->argv[j] != NULL; ++j) {
            printf("\"%s\", ", schedulee->argv[j]);
        }
        printf("\b\b]\n");
    }
    // End debug prints

    // Setting up of core
    cpu_set_t my_set;
    CPU_ZERO(&my_set);
    CPU_SET(0, &my_set);
    sched_setaffinity(0, sizeof(cpu_set_t), &my_set);  //Put in child process

    //Fork child for processes
    pid_t pid_array[schedulees_count];
    pid_t childPid;
    for (int i = 0; i < schedulees_count; i++) {
        childPid = fork();
        pid_array[i] = childPid;
        if (childPid == 0) {  // Child process
            schedulee_t *schedulee = schedulees[i];
            kill(getpid(), SIGSTOP);
            execvp(schedulee->argv[0], schedulee->argv);
        } else if (childPid == -1) {
            fprintf(stderr, "Error creating child\n");
            // should free all
            return 1;
        }
    }
    int step_count = 0;
    int status;
    pid_t return_status;
    int i;

    while (1) {
        int is_done = 1;
        for (i = 0; i < schedulees_count; i++) {
            return_status = waitpid(pid_array[i], &status, WNOHANG);
            if (return_status == 0) {
                is_done = 0;
            }
        }

        if (is_done == 1) {
            printf("done\n");
            break;
        }

        while (step_count < schedulees_count) {
            if (waitpid(pid_array[step_count], &status, WNOHANG) == 0) {
                kill(pid_array[step_count], SIGCONT);
                usleep(tick_interval);
                kill(pid_array[step_count], SIGSTOP);
            }
            step_count++;
        }
        step_count = 0;
    }

    return 0;
}

schedulee_t *make_schedulee(const char *argv)
{
    schedulee_t *ret = calloc(1, sizeof(schedulee_t));
    char *ret_argv_buf = strdup(argv);
    size_t reg_argv_buf_index = 0;
    size_t ret_argv_nmemb = 8;
    size_t ret_argv_index = 0;
    ret->argv = calloc(ret_argv_nmemb, sizeof(char *));
    if (!ret || !ret_argv_buf || !ret->argv) {
        goto fail;
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
                    ret->argv = realloc(ret->argv, ret_argv_nmemb * sizeof(char *));
                    if (!ret->argv) {
                        goto fail;
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

void free_schedulee(schedulee_t *schedulee)
{
    if (schedulee) {
        if (schedulee->argv) {
            free(*schedulee->argv);
            free(schedulee->argv);
        }
        free(schedulee);
    }
}