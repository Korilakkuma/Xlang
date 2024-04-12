#include "parser.h"

struct Node *code[100];

static struct Node *create_node(NodeType type, struct Node *left, struct Node *right);
static struct Node *create_node_with_number(INT value);

void program(void) {
  int i = 0;

  while (!is_tokenize_end()) {
    code[i++] = statement();
  }

  code[i] = NULL;
}

struct Node *statement(void) {
  struct Node *node = expression();

  operator_and_next(";");

  return node;
}

struct Node *expression(void) {
  return assign();
}

struct Node *assign(void) {
  struct Node *node = equality();

  if (test_operator_and_next("=")) {
    node = create_node(NODE_ASSIGN, node, assign());
  }

  return node;
}

struct Node *equality(void) {
  struct Node *node = relational();

  while (true) {
    if (test_operator_and_next("==")) {
      node = create_node(NODE_EQ, node, relational());
    } else if (test_operator_and_next("!=")) {
      node = create_node(NODE_NOT_EQ, node, relational());
    } else {
      break;
    }
  }

  return node;
}

struct Node *relational(void) {
  struct Node *node = term();

  while (true) {
    if (test_operator_and_next("<")) {
      node = create_node(NODE_LESS, node, term());
    } else if (test_operator_and_next("<=")) {
      node = create_node(NODE_LESS_OR_EQUAL, node, term());
    } else if (test_operator_and_next(">")) {
      node = create_node(NODE_GREATER, node, term());
    } else if (test_operator_and_next(">=")) {
      node = create_node(NODE_GREATER_OR_EQUAL, node, term());
    } else {
      break;
    }
  }

  return node;
}

struct Node *term(void) {
  struct Node *node = unary();

  while (true) {
    if (test_operator_and_next("*")) {
      node = create_node(NODE_MUL, node, unary());
    } else if (test_operator_and_next("/")) {
      node = create_node(NODE_DIV, node, unary());
    } else {
      break;
    }
  }

  return node;
}

struct Node *unary(void) {
  if (test_operator_and_next("+")) {
    return factor();
  }

  if (test_operator_and_next("-")) {
    struct Node *zero = create_node_with_number(0);

    return create_node(NODE_SUB, zero, factor());
  }

  return factor();
}

struct Node *factor(void) {
  struct Token *token = identifier_and_next();

  if (isalpha(token->text[0])) {
    struct Node *node = create_node(NODE_LOCAL_VARIABLE, NULL, NULL);

    node->type = NODE_LOCAL_VARIABLE;

    struct LocalVariable *v = find_local_variable(token);

    if (v) {
      node->offset = v->offset;
    } else {
      struct LocalVariable *new_v = (struct LocalVariable *)malloc(1 * sizeof(struct Node));

      new_v->next   = locals;
      new_v->name   = token->text;
      new_v->length = token->length;
      new_v->offset = locals->offset + 8;

      node->offset = new_v->offset;

      locals = new_v;
    }

    return node;
  }

  if (test_operator_and_next("(")) {
    struct Node *node = expression();

    operator_and_next(")");

    return node;
  }

  return create_node_with_number(test_number_and_next());
}

static struct Node *create_node(NodeType type, struct Node *left, struct Node *right) {
  struct Node *node = (struct Node *)malloc(1 * sizeof(struct Node));

  node->type  = type;
  node->left  = left;
  node->right = right;

  return node;
}

static struct Node *create_node_with_number(INT value) {
  struct Node *node = (struct Node *)malloc(1 * sizeof(struct Node));

  node->type  = NODE_NUMBER;
  node->value = value;

  return node;
}
