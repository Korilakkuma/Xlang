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
  RIGHT_PARENTHESES = ')'
} TokenString;

struct Token {
  TokenType type;
  char *text;
  long value;
  struct Token *next;
};

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
