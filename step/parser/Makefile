CC     := gcc
CFLAGS := -std=c99 -g -static -Wall

xlang: xlang.c parser.o lexer.o
	$(CC) $(CFLAGS) -o $@ $^

parser.o: parser.c
	$(CC) $(CFLAGS) -c $<

lexer.o: lexer.c
	$(CC) $(CFLAGS) -c $<

test: xlang
	sh -vx test.sh

clean:
	rm -rf tmp xlang *.o *.s *~

.PHONY: test clean
