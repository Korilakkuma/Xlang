#include "../includes.h"
#include "../types.h"

extern struct LocalVariable *locals;

extern bool test_operator_and_next(char *op);
extern void operator_and_next(char *op);
extern long test_number_and_next(void);
extern bool is_tokenize_end(void);

extern struct Token *identifier_and_next(void);
extern struct LocalVariable *find_local_variable(struct Token *token);

void program(void);
struct Node *statement(void);
struct Node *expression(void);
struct Node *assign(void);
struct Node *equality(void);
struct Node *relational(void);
struct Node *term(void);
struct Node *unary(void);
struct Node *factor(void);
