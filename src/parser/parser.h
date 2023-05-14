#include "../types.h"

static Token look_ahead_token(void);

static void statement(Token *token, FILE *source_file);
static void expression(FILE *source_file);

static void operate(TokenType op);
static void push(int n);
static int pop(void);

static void or_expression(FILE *source_file);
static void and_expression(FILE *source_file);
static void equal_expression(FILE *source_file);
static void relation_expression(FILE *source_file);
static void add_expression(FILE *source_file);
static void multi_expression(FILE *source_file);

static void factor(FILE *source_file);
