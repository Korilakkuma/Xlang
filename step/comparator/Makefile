CC     := gcc
CFLAGS := -std=c99 -g -static -Wall

xlang: xlang.c generator.o parser.o lexer.o processor.o
	$(CC) $(CFLAGS) -o $@ $^

generator.o: generator/generator.c
	$(CC) $(CFLAGS) -c $<

parser.o: parser/parser.c
	$(CC) $(CFLAGS) -c $<

lexer.o: lexer/lexer.c
	$(CC) $(CFLAGS) -c $<

processor.o: utils/processor.c
	$(CC) $(CFLAGS) -c $<

test: xlang
	sh -vx test.sh

clean:
	rm -rf tmp xlang *.o *.s *~

.PHONY: test clean
