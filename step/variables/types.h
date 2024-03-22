#include <sys/types.h>

typedef long INT;

typedef enum {
  Intel32 = 0,
  Intel64,
  ARM32,
  ARM64,
  Others
} PROCESSORS;

// 4 registers x 5 processors x string (char [])
typedef char REGISTERS[5][4];
typedef REGISTERS REGISTERS_MAP[];

typedef enum {
  IDENTIFIER,
  OPERATOR,
  NUMBER,
  EOT
} TokenType;

typedef enum {
  PLUS = '+',
  MINUS = '-',
  MUL = '*',
  DIV = '/',
  LEFT_PARENTHESES = '(',
  RIGHT_PARENTHESES = ')',
  ASSIGN = '=',
  LESS = '<',
  GREATER = '>',
  SEMICOLON = ';'
} TokenString;

struct Token {
  TokenType type;
  char *text;
  long value;
  size_t length;
  struct Token *next;
};

typedef enum {
  NODE_ADD,
  NODE_SUB,
  NODE_MUL,
  NODE_DIV,
  NODE_EQ,
  NODE_NOT_EQ,
  NODE_LESS,
  NODE_LESS_OR_EQUAL,
  NODE_GREATER,
  NODE_GREATER_OR_EQUAL,
  NODE_ASSIGN,
  NODE_LOCAL_VARIABLE,
  NODE_NUMBER
} NodeType;

struct Node {
  NodeType type;
  struct Node *left;
  struct Node *right;
  INT value;
  INT offset;
};
