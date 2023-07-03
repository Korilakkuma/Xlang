#include "parser.h"

static Token token;
static TokenType last_statement;

static Symbol *function_pointer = NULL;
static Symbol tmp_symbol = {"", LITERAL_ID, NON_T, GLOBAL, 0, 0, 0};

static int nest_of_break = 0;

static int counter_for_nest = 0;
static int counter_for_switch = 0;
static int counter_for_case = 0;

static int number_of_errors = 0;

bool compile(FILE *sourcefile) {
  generate_code2(CALL, -1);
  generate_code(STOP);

  token = next_token();

  while (token.type != TOKEN_TYPE_EOF_TOKEN) {
    switch (token.type) {
    case TOKEN_TYPE_INT:
    case TOKEN_TYPE_VOID:
      set_type();
      set_name();

      if (token.type == '(') {
        function_declare();
      } else {
        variable_declare();
      }

      break;
    case TOKEN_TYPE_SEMICOLON:
      token = next_token();
      break;
    default:
      display_error("Syntax Error\n", token.text);
      token = next_token();
    }
  }

  if (counter_for_error == 0) {
    backpatch_calladdress();
  }

  *INT_POINTER(memory_address(0)) = global_malloc(0);

  if (counter_for_error > 0) {
    fprintf(stderr, "%d errors occurred\n", counter_for_error);
  }

  return (bool)(counter_for_error == 0);
}

/*
void variable_declare(void);
void function_declare(void);
void check_function(Symbol *symbol1, Symbol *symbol2);
void function_declare_begin(void);
void function_declare_end(void);
void set_main(void);
void block(bool is_function);
void statement(void);
void continue_or_break_begin(TokenType type);
void continue_or_break_edn(void);
int get_looptop(void);
void switch_begin(void);
void switch_end(void);
void check_expression(TokenType *type1, TokenType *type2);
void expression(void);
void term(int n);
void factor(void);
void call_function(Symbol *symbol);
void call_system_function(TokenType *type);
void set_type(void);
void set_name(void);
void set_array_size(void);
void set_address(Symbol *symbol);
int operation_order(TokenType *type);
int local_malloc(int size);
bool is_global(void);
bool check_format(char *format);
int execute(void);
void dump(void);
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
*/
