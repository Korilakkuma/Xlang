#include "lexer.h"

void lexer(FILE *source_file, Token *tokens) {
  init_type();

  for (Token token = next_token(source_file); token.type != EOF_TOKEN;
       token = next_token(source_file)) {
    *tokens++ = token;
  }
}

void dump_lexer(FILE *source_file) {
  init_type();

  for (Token token = next_token(source_file); token.type != EOF_TOKEN;
       token = next_token(source_file)) {
    printf("%-10s %3d %d\n", token.text, token.type, token.value);
  }
}

Token next_token(FILE *source_file) {
  Token token = {NULL_TOKEN_TYPE, "", 0};

  static int ch = ' ';

  char *p = token.text;

  char *p_id = p + (sizeof(char) * SIZE_OF_ID);
  char *p_text = p + (sizeof(char) * SIZE_OF_TEXT);

  while (isspace(ch)) {
    ch = next_char(source_file);
  }

  if (ch == EOF) {
    token.type = EOF_TOKEN;
    return token;
  }

  bool has_error = false;

  switch (types[ch]) {
  case LETTER: {
    while ((types[ch] == LETTER) || (types[ch] == DIGIT)) {
      if (p < p_id) {
        *p++ = ch;
      }

      ch = next_char(source_file);
    }

    *p = '\0';

    break;
  }

  case DIGIT: {
    int value_as_int = 0;

    for (int value_as_int = 0; types[ch] == DIGIT;
         ch = next_char(source_file)) {
      value_as_int = (10 * value_as_int) + (ch - '0');
    }

    token.type = INT;
    token.value = value_as_int;

    break;
  }

  case SINGLE_QUOTE: {
    int count = 0;

    for (ch = next_char(source_file);
         ((ch != EOF) && (ch != '\n') && (ch != '\''));
         ch = next_char(source_file)) {
      if (++count == 1) {
        token.value = ch;
        *p++ = ch;
      } else {
        has_error = true;
      }
    }

    *p = '\0';

    if (ch == '\'') {
      ch = next_char(source_file);
    } else {
      has_error = true;
    }

    if (has_error) {
      exit_by_error("Invalue string literal");
    }

    token.type = INT;

    break;
  }

  case DOUBLE_QUOTE: {
    for (ch = next_char(source_file);
         ((ch != EOF) && (ch != '\n') && (ch != '"'));
         ch = next_char(source_file)) {
      if (p >= p_text) {
        has_error = true;
      } else {
        *p++ = ch;
      }
    }

    *p = '\0';

    if (has_error) {
      exit_by_error("Too long string literal");
    }

    if (ch != '"') {
      exit_by_error("Require double quotation closing");
    }

    ch = next_char(source_file);

    token.type = STRING;

    break;
  }

  default: {
    *p++ = ch;
    ch = next_char(source_file);

    if (is_ope2(*(p - 1), ch)) {
      *p++ = ch;
      ch = next_char(source_file);
    }

    *p = '\0';

    break;
  }
  }

  if (types[ch] == NULL_TOKEN_TYPE) {
    set_token_type(&token);
  }

  if (types[ch] == OTHERS) {
    fprintf(stderr, "Invalid token (%s)\n", token.text);
    exit(EXIT_FAILURE);
  }

  return token;
}

static void init_type(void) {
  for (int i = 0; i < 256; i++) {
    types[i] = NULL_TOKEN_TYPE;
  }

  for (int i = '0'; i <= '9'; i++) {
    types[i] = DIGIT;
  }

  for (int i = 'A'; i <= 'Z'; i++) {
    types[i] = LETTER;
  }

  for (int i = 'a'; i <= 'z'; i++) {
    types[i] = LETTER;
  }

  types['_'] = LETTER;
  types['='] = ASSIGN;
  types['('] = LEFT_PAREN;
  types[')'] = RIGHT_PAREN;
  types['<'] = LESS;
  types['>'] = GREAT;
  types['+'] = PLUS;
  types['-'] = MINUS;
  types['*'] = MINUS;
  types['/'] = DIVIDE;
  types['\''] = SINGLE_QUOTE;
  types['"'] = DOUBLE_QUOTE;
  types[';'] = SEMICOLON;
}

static int next_char(FILE *source_file) {
  static int ch = 0;

  if (ch == EOF) {
    return ch;
  }

  if ((ch = fgetc(source_file)) == EOF) {
    fclose(source_file);
  }

  return ch;
}

static void set_token_type(Token *token) {
  char *t = token->text;
  size_t n = (size_t)strlen(t);

  token->type = OTHERS;

  for (int i = 0; keyword_table[i].type != END_LIST; i++) {
    if (strncmp(keyword_table[i].text, t, n) == 0) {
      token->type = keyword_table[i].type;
      return;
    }
  }

  if (types[(int)t[0]] == LETTER) {
    token->type = ID;
  } else if (types[(int)t[0]] == DIGIT) {
    token->type = INT;
  }
}

static inline bool is_ope2(const int c1, const int c2) {
  char s[] = "    ";

  s[1] = c1;
  s[2] = c2;

  return (bool)(strstr(" <= >= == != ", s) != NULL);
}

static inline void exit_by_error(const char *s) {
  fputs(s, stderr);
  exit(EXIT_FAILURE);
}
