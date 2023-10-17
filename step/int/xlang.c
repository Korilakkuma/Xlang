#include <stdio.h>
#include <stdlib.h>
#include "../../utils/processor.c"

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./xlang 0-255\n", stderr);
    exit(EXIT_FAILURE);
  }

  char r0[4];

  get_r0_register_name(r0);

  PROCESSORS processor_code = get_processor_code();

  if ((processor_code == Intel32) || (processor_code == Intel64)) {
    fprintf(stdout, ".intel_syntax noprefix\n");
  }

  fprintf(stdout, ".globl main\n");
  fprintf(stdout, "main:\n");
  fprintf(stdout, "		mov %s, %d\n", r0, (int)strtol(argv[1], NULL, 10));
  fprintf(stdout, "		ret\n");

  return 0;
}
