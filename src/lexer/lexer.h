#include "../types.h"

static void init_type(void);
static int next_char(FILE *source_file);
static void set_token_type(Token *token);

static inline bool is_ope2(const int c1, const int c2);
static inline void exit_by_error(const char *s);

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
