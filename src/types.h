#include "includes.h"
#include "macros.h"

typedef struct {
  TokenType type;
  char text[SIZE_OF_TEXT];
  int value;
} Token;

typedef struct {
  char *symbol;
  SymbolType symbol_type;
  DataType type;
  int length;
  Memory memory;
  char argc;
  int address;
} Symbol;

