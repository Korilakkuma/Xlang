#include "includes.h"
#include "types.h"

static char *input;

extern void tokenize(char *p);
extern struct Node *expression(void);
extern void generate(struct Node *node);
