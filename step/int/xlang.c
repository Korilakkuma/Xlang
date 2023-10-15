#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./xlang 0-255\n", stderr);
    exit(EXIT_FAILURE);
  }

  // fprintf(stdout, ".intel_syntax noprefix\n");
  fprintf(stdout, ".globl main\n");
  fprintf(stdout, "main:\n");
  // fprintf(stdout, "		mov rax, %d\n", (int)strtol(argv[1], NULL, 10));
  fprintf(stdout, "		mov w0, %d\n", (int)strtol(argv[1], NULL, 10));
  fprintf(stdout, "		ret\n");

  return 0;
}
