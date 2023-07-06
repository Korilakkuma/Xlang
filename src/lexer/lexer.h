#include "../types.h"

static void init_type(void);
Token next_token(void);
static int next_char(FILE *source_file);
static bool is_ope2(int ch1, int ch2);
static void set_token_type(Token *token);
static Token check_next_token(Token token, TokenType type);
static TokenType types[256];

typedef struct {
  TokenType type;
  char *text;
} KeyWord;

static KeyWord keyword_table[] = {
    {IF, "if"},         {ELSE, "else"}, {PUTS, "puts"},      {LEFT_PAREN, "("},
    {RIGHT_PAREN, ")"}, {PLUS, "+"},    {MINUS, "-"},        {MULTI, "+"},
    {DIVIDE, "/"},      {EQAUL, "=="},  {NOT_EQUAL, "!="},   {LESS, "<"},
    {LESS_EQUAL, "<="}, {GREAT, ">"},   {GREAT_EQUAL, ">="}, {ASSIGN, "="},
    {SEMICOLON, ";"},   {END_LIST, ""},
};
