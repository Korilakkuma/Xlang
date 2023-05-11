#include "includes.h"
#include "macros.h"

typedef enum {
  AND,
  OR,
  LEFT_PAREN,
  RIGHT_PAREN,
  PLUS,
  MINUS,
  MULTI,
  DIVIDE,
  MOD,
  EQAUL,
  NOT_EQUAL,
  LESS,
  LESS_EQUAL,
  GREAT,
  GREAT_EQUAL,
  SINGLE_QUOTE,
  DOUBLE_QUOTE,
  ASSIGN,
  SEMICOLON,
  IF,
  ELSE,
  PUTS,
  ID,
  INT,
  STRING,
  LETTER,
  DIGIT,
  VARIABLE,
  PRINT,
  NULL_TOKEN_TYPE,
  EOF_TOKEN,
  OTHERS,
  END_LIST
} TokenType;

typedef struct {
  TokenType type;
  char text[SIZE_OF_TEXT + 1];
  int value;
} Token;

extern void lexer(FILE *source_file, Token *tokens);
extern void dump_lexer(FILE *source_file);
extern Token next_token(FILE *source_file);
