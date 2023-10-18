#include <stdio.h>
#include <stdlib.h>
#include "../../utils/processor.c"

enum {
  ADD = '+',
  SUB = '-'
} OPERATORS;

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./xlang ([0-9]+[+-][0-9])+\n", stderr);
    exit(EXIT_FAILURE);
  }

  REGISTERS registers;

  get_registers(registers);

  PROCESSORS processor_code = get_processor_code();

  if ((processor_code == Intel32) || (processor_code == Intel64)) {
    fprintf(stdout, ".intel_syntax noprefix\n");
  }

  const char *r0 = registers[0];
  const char *r1 = registers[1];

  char *p = argv[1];

  long operand1 = strtol(p, &p, 10);

  fprintf(stdout, ".globl main\n");
  fprintf(stdout, "main:\n");
  fprintf(stdout, "		mov %s, %ld\n", r0, operand1);

  while (*p) {
    switch (p[0]) {
      case ADD: {
        ++p;

        long operand2 = strtol(p, &p, 0);

        if ((processor_code == Intel32) || (processor_code == Intel64)) {
          fprintf(stdout, "		add %s, %ld\n", r0, operand2);
        } else {
          fprintf(stdout, "		add %s, %s, %ld\n", r0, r0, operand2);
        }

        break;
      }

      case SUB: {
        ++p;

        long operand2 = strtol(p, &p, 0);

        if ((processor_code == Intel32) || (processor_code == Intel64)) {
          fprintf(stdout, "		sub %s, %ld\n", r0, operand2);
        } else {
          fprintf(stdout, "		sub %s, %s, %ld\n", r0, r0, operand2);
        }

        break;
      }

      default: {
        fputs("Invalid expression\n", stderr);
        exit(EXIT_FAILURE);
      }
    }
  }

  fprintf(stdout, "		ret\n");

  return 0;
}
