#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct schedulee {
  int proportion;
  char **argv;
} schedulee_t;
schedulee_t *make_schedulee(int, const char *);
void free_schedulee(schedulee_t *);

int main(int argc, char *argv[]) {
  int exit_code = 0;
  int u_interval = 100, n_cores = 1;
  char opt;
  while ((opt = getopt(argc, argv, "u:")) != -1) {
    switch (opt) {
    case 'u':
      u_interval = atoi(optarg);
      break;
    default:
      fprintf(stderr, "Usage: %s [-u U] < config_file\n", argv[0]);
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
    goto invalid_config;
  }

  while (getline(&line, &line_size, stdin) != -1) {
    int argv_offset = 0;
    int proportion = 0;
    if (sscanf(line, "%d %n", &proportion, &argv_offset) != 1) {
      goto invalid_config;
    }
    char *schedulee_argv = line + argv_offset;
    schedulee_t *schedulee = make_schedulee(proportion, schedulee_argv);
    if (!schedulee) {
      goto fail;
    }
    if (schedulees_count >= schedulees_size) {
      schedulees_size += 8;
      schedulees = realloc(schedulees, schedulees_size*sizeof(schedulee_t *));
      if (!schedulees) {
        goto fail;
      }
    }
    schedulees[schedulees_count++] = schedulee;
  }

  // Remove the following debug prints from your solution
  printf("u_interval=%d\n", u_interval);
  for (size_t i = 0; i < schedulees_count; ++i) {
    schedulee_t *schedulee = schedulees[i];
    printf("schedulee %zu: %d%%\n  argv: [", i, schedulee->proportion);
    for (size_t j = 0; schedulee->argv[j] != NULL; ++j) {
      printf("\"%s\", ", schedulee->argv[j]);
    }
    printf("\b\b]\n");
  }
  // End debug prints

  // Your code here

  goto exit;
invalid_config:
  fprintf(stderr, "Invalid configuration file\n");
fail:
  exit_code = 1;
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

schedulee_t *make_schedulee(int proportion, const char *argv) {
  schedulee_t *ret = calloc(1, sizeof(schedulee_t));
  char *ret_argv_buf = strdup(argv);
  size_t reg_argv_buf_index = 0;
  size_t ret_argv_nmemb = 8;
  size_t ret_argv_index = 0;
  ret->proportion = proportion;
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
          ret->argv = realloc(ret->argv, ret_argv_nmemb*sizeof(char *));
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

void free_schedulee(schedulee_t *schedulee) {
  if (schedulee) {
    if (schedulee->argv) {
      free(*schedulee->argv);
      free(schedulee->argv);
    }
    free(schedulee);
  }
}
