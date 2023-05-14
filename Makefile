CC      := gcc
CFLAGS  := -std=c99 -O2 -Wall

all     : bin/x
bin/x   : main.o lexer.o parser.o
	$(CC) $^ -o $@
main.o  : cli/main.c
	$(CC) -c $(CFLAGS) $<
lexer.o : src/lexer/lexer.c
	$(CC) -c $(CFLAGS) $<
parser.o : src/parser/parser.c
	$(CC) -c $(CFLAGS) $<

format  :
	find cli src -name "*.c" -o -name "*.h" | xargs clang-format --verbose -style=LLVM -i

clean   :
	rm -rf *.s *.o *.out bin/x
