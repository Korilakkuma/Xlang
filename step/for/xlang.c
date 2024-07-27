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
  program();

  if ((processor_code == Intel32) || (processor_code == Intel64)) {
    fprintf(stdout, ".intel_syntax noprefix\n");
  }

  fprintf(stdout, ".globl main\n");
  fprintf(stdout, "main:\n");

  fprintf(stdout, "		push rbp\n");
  fprintf(stdout, "		mov rbp, rsp\n");
  fprintf(stdout, "		sub rsp, 200\n");

  for (int i = 0; code[i]; i++) {
    generate(code[i]);

    fprintf(stdout, "		pop %s\n", r0);
  }

  fprintf(stdout, "		mov rsp, rbp\n");
  fprintf(stdout, "		pop rbp\n");
  fprintf(stdout, "		ret\n");

  return 0;
}
