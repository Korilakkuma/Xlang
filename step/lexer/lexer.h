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
