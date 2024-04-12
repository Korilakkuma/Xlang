#include "../includes.h"
#include "../types.h"

void tokenize(char *p);
bool test_operator_and_next(char *op);
void operator_and_next(char *op);
long test_number_and_next(void);
bool is_tokenize_end(void);

struct Token *identifier_and_next(void);
struct LocalVariable *find_local_variable(struct Token *token);
