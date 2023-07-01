#include "../src/types.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./bin/x [--code] source\n", stderr);
    exit(EXIT_FAILURE);
  }

  FILE *sourcefile = fopen(argv[1], "r");

  if (sourcefile == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  if (compile(sourcefile)) {
    if ((argc == 3) && (strncmp(argv[2], "--code", 6) == 0)) {
      dump();

      return 0;
    }

    return execute();
  }

  return 1;
}
