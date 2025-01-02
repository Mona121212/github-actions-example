CC = gcc
CFLAGS = -g -Wall -Wextra -fsanitize=address -fno-omit-frame-pointer
LDFLAGS = -fsanitize=address

.PHONY: all clean run debug test

all: text_analyzer

text_analyzer: text_analyzer.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

run: text_analyzer
	./text_analyzer "The quick brown fox jumps over the lazy dog"

test: text_analyzer test_input.py
	python3 test_input.py | ./text_analyzer

debug: text_analyzer
	gdb ./text_analyzer

clean:
	rm -f text_analyzer
