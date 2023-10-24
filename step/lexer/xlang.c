#include "lexer.c"
#include "../../utils/processor.c"

extern struct Token *tokenize(char *p);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./xlang ([0-9]|\\s*+\\s*[+-][0-9])+\n", stderr);
    exit(EXIT_FAILURE);
  }

  REGISTERS registers;

  get_registers(registers);

  PROCESSORS processor_code = get_processor_code();

  const char *r0 = registers[0];

  char *p = argv[1];

  struct Token *tokens = tokenize(p);

  if ((processor_code == Intel32) || (processor_code == Intel64)) {
    fprintf(stdout, ".intel_syntax noprefix\n");
  }

  fprintf(stdout, ".globl main\n");
  fprintf(stdout, "main:\n");
  fprintf(stdout, "		mov %s, %ld\n", r0, test_number_and_next(&tokens));

  while (!is_tokenize_end(&tokens)) {
    if (test_operator_and_next(&tokens, PLUS)) {
      if ((processor_code == Intel32) || (processor_code == Intel64)) {
        fprintf(stdout, "		add %s, %ld\n", r0, test_number_and_next(&tokens));
      } else {
        fprintf(stdout, "		add %s, %s, %ld\n", r0, r0, test_number_and_next(&tokens));
      }
    } else if (test_operator_and_next(&tokens, MINUS)) {
      if ((processor_code == Intel32) || (processor_code == Intel64)) {
        fprintf(stdout, "		sub %s, %ld\n", r0, test_number_and_next(&tokens));
      } else {
        fprintf(stdout, "		sub %s, %s, %ld\n", r0, r0, test_number_and_next(&tokens));
      }
    }
  }

  fprintf(stdout, "		ret\n");

  return 0;
}
