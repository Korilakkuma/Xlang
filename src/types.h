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

typedef enum {
  LITERAL_ID,
  VARIABLE_ID,
  FUNCTION_ID,
  PROTOTYPE_ID,
  ARGUMENT_ID
} SymbolType;

typedef struct {
  TokenType type;
  char text[SIZE_OF_TEXT + 1];
  int value;
} Token;

typedef enum { NON_T, VOID_T, INT_T } DataType;

/**
 * Text Segment
 * -------------
 * Data Segment
 * - PPDA (Per Process Data Area)
 * - Data Area
 *  - Static Area
 *  - Heap Area
 * - Stack Area
 */
typedef enum {
  GLOBAL, // Data Area
  LOCAL   // Stack Area
} Memory;

typedef struct {
  char *symbol;
  SymbolType symbol_type;
  DataType type;
  int length;
  Memory memory;
  char argc;
  int address;
} Symbol;

Symbol symbol_table[SIZE_OF_SYMBOL_TABLE];
Symbol tmp_symbol;

extern void lexer(FILE *source_file, Token *tokens);
extern void dump_lexer(FILE *source_file);
extern Token next_token(FILE *source_file);
