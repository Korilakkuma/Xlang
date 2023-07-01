#include "includes.h"
#include "macros.h"

typedef struct {
  TokenType type;
  char text[SIZE_OF_TEXT];
  int value;
} Token;

typedef struct {
  char *symbol;
  SymbolType symbol_type;
  DataType type;
  int length;
  Memory memory;
  char argc;
  int address;
} Symbol;

typedef struct {
  unsigned char opcode;
  unsigned char flag;
  int opdata;
} Instruction;

extern bool compile(FILE *sourcefile);
extern void variable_declare(void);
extern void function_declare(void);
extern void check_function(Symbol *symbol1, Symbol *symbol2);
extern void function_declare_begin(void);
extern void function_declare_end(void);
extern void set_main(void);
extern void block(bool is_function);
extern void statement(void);
extern void continue_or_break_begin(TokenType type);
extern void continue_or_break_edn(void);
extern int get_looptop(void);
extern void switch_begin(void);
extern void switch_end(void);
extern void check_expression(TokenType *type1, TokenType *type2);
extern void expression(void);
extern void term(int n);
extern void factor(void);
extern void call_function(Symbol *symbol);
extern void call_system_function(TokenType *type);
extern void set_type(void);
extern void set_name(void);
extern void set_array_size(void);
extern void set_address(Symbol *symbol);
extern int operation_order(TokenType *type);
extern int local_malloc(int size);
extern bool is_global(void);
extern bool check_format(char *format);
extern int execute(void);
extern void dump(void);
extern int get_line_number(void);
extern int execute(void);
extern void dump(void);
extern int is_kanji(const int ch);
extern int get_kanji_mode(void);
extern char *string_malloc(const char *s);
extern void increment(int *var, int size, const char *error_message);
extern void display_error(const char *error_message1,
                          const char *error_message2);
extern void display_single_error(const char *error_message);
extern void display_formatted_error(const char *format, int id);
