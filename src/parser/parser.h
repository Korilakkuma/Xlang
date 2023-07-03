#include "../types.h"

int local_address;

#define START_LOCAL_ADDRESS (1 * SIZE_OF_INT)

#define INT_POINTER (int *)(p)

#define SIZE_OF_LOOP 20
#define SIZE_OF_SWITCH 10
#define SIZE_OF_CASE 100

static struct {
  TokenType type;
  int top;
  bool has_break;
} loop_nests[SIZE_OF_LOOP];

static struct {
  int default_address;
  int start_case_list;
} switch_nests[SIZE_OF_SWITCH];

static struct {
  int value;
  int address;
} cases[SIZE_OF_CASE];

extern Token next_token(void);
extern void generate_code3(OperationCode op, int data, int address);
extern void display_error(const char *error_message1,
                          const char *error_message2);
