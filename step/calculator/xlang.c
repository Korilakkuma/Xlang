#include "xlang.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./parser [0-9]+(+-*/)[0-9]+\n", stderr);
    exit(EXIT_FAILURE);
  }

  char r0[4];

  get_r0_register_name(r0);

  PROCESSORS processor_code = get_processor_code();

  tokenize(argv[1]);

  struct Node *node = expression();

  if ((processor_code == Intel32) || (processor_code == Intel64)) {
    fprintf(stdout, ".intel_syntax noprefix\n");
  }

  fprintf(stdout, ".globl main\n");
  fprintf(stdout, "main:\n");

  generate(node);

  fprintf(stdout, "		pop %s\n", r0);
  fprintf(stdout, "		ret\n");

  return 0;
}
