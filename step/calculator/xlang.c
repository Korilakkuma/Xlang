#include "xlang.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./parser [0-9]+(+-*/)[0-9]+\n", stderr);
    exit(EXIT_FAILURE);
  }

  tokenize(argv[1]);

  struct Node *node = expression();

  fprintf(stdout, ".intel_syntax noprefix\n");
  fprintf(stdout, ".globl main\n");
  fprintf(stdout, "main:\n");

  generate(node);

  fprintf(stdout, "		pop rax\n");
  fprintf(stdout, "		ret\n");

  return 0;
}
