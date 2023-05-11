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

  Token *tokens = malloc(MAX_TOKENS * sizeof(Token));

  lexer(source_file, tokens);

  Token token;

  int p = 0;

  do {
    token = tokens[p++];
    printf("%-10s %3d %d\n", token.text, token.type, token.value);
  } while (strncmp("", token.text, 1) != 0);

  free(tokens);

  return 0;
}
