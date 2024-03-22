#include "../includes.h"
#include "../types.h"

void tokenize(char *p);
bool test_operator_and_next(char *op);
void operator_and_next(char *op);
long test_number_and_next(void);
char identifier_and_next(void);
bool is_tokenize_end(void);
