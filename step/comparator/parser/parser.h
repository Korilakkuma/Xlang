#include "../includes.h"
#include "../types.h"

extern bool test_operator_and_next(char *op);
extern void operator_and_next(char *op);
extern long test_number_and_next(void);

struct Node *expression(void);
struct Node *term(void);
struct Node *unary(void);
struct Node *factor(void);
