#include "parser.h"
#include <stdio.h>

#define SIZE_OF_STACK 20

Token token;

int stack[SIZE_OF_STACK];
int stack_pointer = 0;

int vars[10000];

void parser(void) {}

static Token look_ahead_token(void) {
  Token token;

  return token;
}

static void statement(Token *token, FILE *source_file) {
  switch (token->type) {
  case VARIABLE:
    expression(source_file);
    break;
  case PRINT:
    expression(source_file);
    break;
  default:
    break;
  }
}

static void expression(FILE *source_file) {
  if ((token.type == VARIABLE) && (look_ahead_token().type == ASSIGN)) {
    int value = token.value;

    token = next_token(source_file);
    token = next_token(source_file);

    expression(source_file);

    vars[value] = stack[stack_pointer - 1];
  } else {
    or_expression(source_file);
  }
}

static void operate(TokenType op) {
  int op2 = pop();
  int op1 = pop();

  if ((op == DIVIDE) && (op2 == 0)) {
    fputs("Zero Division Error\n", stderr);
    return;
  }

  switch (op) {
  case PLUS:
    push(op1 + op2);
    break;
  case MINUS:
    push(op1 - op2);
    break;
  case MULTI:
    push(op1 * op2);
    break;
  case DIVIDE:
    push(op1 / op2);
    break;
  default:
    break;
  }
}

static void push(int n) {
  if (stack_pointer >= SIZE_OF_STACK) {
    fputs("Stack overflow\n", stderr);
    exit(EXIT_FAILURE);
  }

  stack[stack_pointer++] = n;
}

static int pop(void) {
  if (stack_pointer <= 0) {
    fputs("Stack is empty\n", stderr);
    exit(EXIT_FAILURE);
  }

  return stack[--stack_pointer];
}
static void or_expression(FILE *source_file) {
  and_expression(source_file);

  while (token.type == OR) {
    token = next_token(source_file);

    and_expression(source_file);
    operate(OR);
  }
}

static void and_expression(FILE *source_file) {
  equal_expression(source_file);

  while (token.type == AND) {
    token = next_token(source_file);

    equal_expression(source_file);
    operate(AND);
  }
}

static void equal_expression(FILE *source_file) {
  TokenType op;

  relation_expression(source_file);

  while ((token.type == EQAUL) || (token.type == NOT_EQUAL)) {
    op = token.type;

    token = next_token(source_file);

    relation_expression(source_file);
    operate(op);
  }
}

static void relation_expression(FILE *source_file) {
  TokenType op;

  add_expression(source_file);

  while ((token.type == LESS) || (token.type == LESS_EQUAL) ||
         (token.type == GREAT) || (token.type == GREAT_EQUAL)) {
    op = token.type;

    token = next_token(source_file);

    add_expression(source_file);
    operate(op);
  }
}

static void add_expression(FILE *source_file) {
  TokenType op;

  multi_expression(source_file);

  while ((token.type == PLUS) || (token.type == MINUS)) {
    op = token.type;

    token = next_token(source_file);

    multi_expression(source_file);
    operate(op);
  }
}

static void multi_expression(FILE *source_file) {
  TokenType op;

  factor(source_file);

  while ((token.type == MULTI) || (token.type == DIVIDE) ||
         (token.type == MOD)) {
    op = token.type;

    token = next_token(source_file);

    factor(source_file);
    operate(op);
  }
}

static void factor(FILE *source_file) {
  switch (token.type) {
  case VARIABLE: {
    push(vars[token.value]);
    break;
  }

  case INT: {
    push(token.value);
    break;
  }

  case LEFT_PAREN: {
    token = next_token(source_file);
    expression(source_file);
    break;
  }

  default: {
    break;
  }
  }

  token = next_token(source_file);
}
