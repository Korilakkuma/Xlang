#include "lexer.h"

static struct Token *current_token;

static struct Token *create_token(TokenType type, char *text);
static void handle_lexer_error(char *fmt, ...);

struct Token *tokenize(char *p) {
  struct Token head;

  head.next = NULL;

  current_token = &head;

  while (*p) {
    if (isspace(p[0])) {
      ++p;
      continue;
    }

    if (isdigit(p[0])) {
      current_token = create_token(NUMBER, p);
      current_token->value = strtol(p, &p, 10);
      continue;
    }

    switch (p[0]) {
      case PLUS :
      case MINUS: {
        current_token = create_token(OPERATOR, p);
        ++p;
        break;
      }

      default: {
        break;
      }
    }
  }

  create_token(EOT, p);

  return head.next;
}

bool test_operator_and_next(char op) {
  if ((current_token->type != OPERATOR) || (current_token->text[0] != op)) {
    return false;
  }

  current_token = current_token->next;

  return true;
}

void operator_and_next(char op) {
  if ((current_token->type!= OPERATOR) || (current_token->text[0] != op)) {
    handle_lexer_error("'%c' is not %c\n", current_token->text[0], op);
  }

  current_token = current_token->next;
}

long test_number_and_next(void) {
  if (current_token->type != NUMBER) {
    handle_lexer_error("'%s' is not number\n", current_token->text);
    exit(EXIT_FAILURE);
  }

  long value = current_token->value;

  current_token = current_token->next;

  return value;
}

bool is_tokenize_end(struct Token **current_token) {
  return (**current_token).type == EOT;
}

static struct Token *create_token(TokenType type, char *text) {
  struct Token *token = (struct Token*)malloc(sizeof(struct Token));

  token->type = type;
  token->text = text;

  if (type == NUMBER) {
    token->value = strtol(text, NULL, 10);
  }

  current_token->next = token;

  return token;
}

static void handle_lexer_error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  exit(EXIT_FAILURE);
}
