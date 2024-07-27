#include "lexer.h"

struct LocalVariable *locals;

static struct Token *current_token;
static char *input;
static char *current_input;

static struct Token *create_token(TokenType type, struct Token *current, char *text, size_t length);
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

    if ((strncmp(p, "==", 2) == 0) || (strncmp(p, "!=", 2) == 0)) {
      current_token = create_token(OPERATOR, current_token, p, 2);
      p += 2;
      current_input += 2;
      continue;
    }

    if ((strncmp(p, "<=", 2) == 0) || (strncmp(p, ">=", 2) == 0)) {
      current_token = create_token(OPERATOR, current_token, p, 2);
      p += 2;
      current_input += 2;
      continue;
    }

    bool is_operator = false;

    switch (p[0]) {
      case PLUS :
      case MINUS:
      case MUL  :
      case DIV  : {
        current_token = create_token(OPERATOR, current_token, p++, 1);
        current_input = p;
        is_operator = true;
        break;
      }

      case LEFT_PARENTHESES :
      case RIGHT_PARENTHESES: {
        current_token = create_token(OPERATOR, current_token, p++, 1);
        current_input = p;
        is_operator = true;
        break;
      }

      case ASSIGN: {
        current_token = create_token(OPERATOR, current_token, p++, 1);
        current_input = p;
        is_operator = true;
        break;
      }

      case LESS   :
      case GREATER: {
        current_token = create_token(OPERATOR, current_token, p++, 1);
        current_input = p;
        is_operator = true;
        break;
      }

      case SEMICOLON: {
        current_token = create_token(OPERATOR, current_token, p++, 1);
        current_input = p;
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
      current_token = create_token(NUMBER, current_token, p, 1);
      current_token->value = strtol(p, &p, 10);
      current_input = p;
      continue;
    }

    if ((strncmp(p, "if", 2) == 0) && (p[2] == '(')) {
      current_token = create_token(IF, current_token, p, 2);
      p += 2;
      current_input = p;
      continue;
    }

    if ((strncmp(p, "else", 4) == 0) && (p[4] == ' ')) {
      current_token = create_token(ELSE, current_token, p, 4);
      p += 4;
      current_input = p;
      continue;
    }

    if ((strncmp(p, "while", 5) == 0) && (p[5] == '(')) {
      current_token = create_token(WHILE, current_token, p, 5);
      p += 5;
      current_input = p;
      continue;
    }

    if ((strncmp(p, "for", 3) == 0) && (p[3] == '(')) {
      current_token = create_token(FOR, current_token, p, 3);
      p += 3;
      current_input = p;
      continue;
    }

    if ((strncmp(p, "return", 6) == 0) && ((p[6] == ' ') || (p[6] == ';'))) {
      current_token = create_token(RETURN, current_token, p, 6);
      p += 6;
      current_input = p;
      continue;
    }

    if ((*p >= 'a') && (*p <= 'z')) {
      char identifier[26];

      while (isalpha(*p)) {
        strncat(identifier, p++, 1);
      }

      identifier[strlen(identifier) + 1] = '\0';

      current_token = create_token(IDENTIFIER, current_token, identifier, strlen(identifier));
      current_input = p;
      continue;
    }

    handle_lexer_error(p, "Cannot Tokenize\n");
  }

  create_token(EOT, current_token, p, 1);

  current_token = head.next;
}

bool test_operator_and_next(char *op) {
  if ((current_token->type == IF) && (strncmp(op, "if", 2) == 0)) {
    current_token = current_token->next;

    return true;
  }

  if ((current_token->type == ELSE) && (strncmp(op, "else", 4) == 0)) {
    current_token = current_token->next;

    return true;
  }

  if ((current_token->type == WHILE) && (strncmp(op, "while", 5) == 0)) {
    current_token = current_token->next;

    return true;
  }

  if ((current_token->type == FOR) && (strncmp(op, "for", 3) == 0)) {
    current_token = current_token->next;

    return true;
  }

  if ((current_token->type != OPERATOR) || (strlen(op) != current_token->length) || memcmp(current_token->text, op, current_token->length)) {
    return false;
  }

  current_token = current_token->next;

  return true;
}

void operator_and_next(char *op) {
  if ((current_token->type != OPERATOR) || (strlen(op) != current_token->length) || memcmp(current_token->text, op, current_token->length)) {
    if (op[0] != ';') {
      handle_lexer_error("'%s' is not %s\n", current_token->text, op);
    }
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

struct Token *identifier_and_next(void) {
  struct Token *token = current_token;

  if (isalpha(token->text[0])) {
    struct LocalVariable *v = (struct LocalVariable *)malloc(1 * sizeof(struct LocalVariable));

    v->name   = token->text;
    v->length = token->length;

    locals = v;

    current_token = current_token->next;
  }

  return token;
}

bool is_tokenize_end(void) {
  return current_token->type == EOT;
}

struct LocalVariable *find_local_variable(struct Token *token) {
  for (struct LocalVariable *v = locals; v != NULL; v = v->next) {
    if ((token->length == v->length) && !memcmp(token->text, v->name, v->length)) {
      return v;
    }
  }

  return NULL;
}

static struct Token *create_token(TokenType type, struct Token *current, char *text, size_t length) {
  struct Token *token = (struct Token*)malloc(sizeof(struct Token));

  token->type   = type;
  token->text   = text;
  token->length = (size_t)length;

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
