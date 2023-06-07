#include "types.h"

bool compile(FILE *source_file) {
  Token token = next_token(source_file);

  while (token.type != EOF_TOKEN) {
    switch (token.type) {
    case INT:
    case VOID_T: {
      break;
    }

    case SEMICOLON: {
      break;
    }

    default: {
      break;
    }
    }

    token = next_token(source_file);
  }

  return true;
}
