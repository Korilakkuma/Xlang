#include "includes.h"
#include "types.h"

extern void get_r0_register_name(char *r0);
extern PROCESSORS get_processor_code(void);
extern void tokenize(char *p);
extern struct Node *expression(void);
extern void generate(struct Node *node);
