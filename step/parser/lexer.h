#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>

typedef enum {
  IDENTIFIER,
  OPERATOR,
  NUMBER,
  EOT
} TokenType;

typedef enum {
  PLUS = '+',
  MINUS = '-'
} TokenString;

struct Token {
  TokenType type;
  char *text;
  long value;
  struct Token *next;
};

struct Token *tokenize(char *p);
bool test_operator_and_next(char op);
void operator_and_next(char op);
long test_number_and_next(void);
bool is_tokenize_end(struct Token **current_token);
