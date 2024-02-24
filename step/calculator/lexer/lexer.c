#include "lexer.h"

static struct Token *current_token;
static char *input;
static char *current_input;

static struct Token *create_token(TokenType type, struct Token *current, char *text);
static void handle_lexer_error(char *fmt, ...);

void tokenize(char *p) {
  struct Token head;

  head.next = NULL;

  current_token = &head;

  input         = p;
  current_input = p;

  while (*p) {
    if (isspace(p[0])) {
      ++p;
      ++current_input;
      continue;
    }

    bool is_operator = false;

    switch (p[0]) {
      case PLUS :
      case MINUS:
      case MUL  :
      case DIV  :
      case LEFT_PARENTHESES :
      case RIGHT_PARENTHESES: {
        current_token = create_token(OPERATOR, current_token, p++);
        ++current_input;
        is_operator = true;
        break;
      }

      default: {
        break;
      }
    }

    if (is_operator) {
      continue;
    }

    if (isdigit(p[0])) {
      current_token = create_token(NUMBER, current_token, p);
      current_token->value = strtol(p, &p, 10);
      current_input = p;
      continue;
    }

    handle_lexer_error(p, "Cannot Tokenize\n");
  }

  create_token(EOT, current_token, p);

  current_token = head.next;
}

bool test_operator_and_next(char op) {
  if ((current_token->type != OPERATOR) || (current_token->text[0] != op)) {
    return false;
  }

  current_token = current_token->next;

  return true;
}

void operator_and_next(char op) {
  if ((current_token->type != OPERATOR) || (current_token->text[0] != op)) {
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

bool is_tokenize_end(void) {
  return current_token->type == EOT;
}

static struct Token *create_token(TokenType type, struct Token *current, char *text) {
  struct Token *token = (struct Token*)malloc(sizeof(struct Token));

  token->type = type;
  token->text = text;

  current->next = token;

  return token;
}

static void handle_lexer_error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int p = current_input - input;

  fprintf(stderr, "%s\n", input);
  fprintf(stderr, "%*s", p, " ");
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");

  va_end(ap);
  exit(EXIT_FAILURE);
}
