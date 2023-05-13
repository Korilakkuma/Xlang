#include "../src/types.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./bin/x source\n", stderr);
    exit(EXIT_FAILURE);
  }

  FILE *source_file = fopen(argv[1], "r");

  if (source_file == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  dump_lexer(source_file);

  return 0;
}
