CC     := gcc
CFLAGS := -std=c99 -g -static -Wall

xlang: xlang.c
	$(CC) $(CFLAGS) -o $@ $^

test: xlang
	sh -vx test.sh

clean:
	rm -rf tmp xlang *.o *.s *~

.PHONY: test clean
