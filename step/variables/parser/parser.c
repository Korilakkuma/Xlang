#include "parser.h"

static struct Node *create_node(NodeType type, struct Node *left, struct Node *right);
static struct Node *create_node_with_number(INT value);

struct Node *expression(void) {
  struct Node *node = term();

  while (true) {
    if (test_operator_and_next("+")) {
      node = create_node(NODE_ADD, node, term());
    } else if (test_operator_and_next("-")) {
      node = create_node(NODE_SUB, node, term());
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
