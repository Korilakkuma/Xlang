#include <stdlib.h>
#include "./lexer.c"

typedef long INT;

typedef enum {
  NODE_ADD,
  NODE_SUB,
  NODE_MUL,
  NODE_DIV,
  NODE_NUMBER
} NodeType;

struct Node {
  NodeType type;
  struct Node *left;
  struct Node *right;
  INT value;
};

struct Node *expression(void);
struct Node *term(void);
struct Node *factor(void);
