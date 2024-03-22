#include "includes.h"
#include "types.h"

extern struct Node *code[];

extern void get_r0_register_name(char *r0);
extern PROCESSORS get_processor_code(void);
extern void tokenize(char *p);
extern void program(void);
extern void generate(struct Node *node);
