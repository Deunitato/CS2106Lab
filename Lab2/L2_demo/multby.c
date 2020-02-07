#include <stdio.h>

int main(int argv, char *argc[]) {
  if (argv < 2) {
    fprintf(stderr, "Usage: %s <factor>\n", argc[0]);
    return 1;
  }

  size_t a, b;
  sscanf(argc[1], "%zu", &a);
  if (scanf("%zu", &b) != 1) {
    fprintf(stderr, "No input\n");
    return 1;
  }

  printf("%zu", a*b);
  return 0;
}
