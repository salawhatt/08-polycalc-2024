CC = gcc
CFLAGS = -Wall -Wextra
LDLIBS = -lm

.PHONY: all all_clean clean
all: PolyCalc

PolyCalc: main.c polycalc.c lex.yy.c parser.tab.c parser.tab.h
	$(CC) $(CFLAGS) -o $@ main.c polycalc.c lex.yy.c parser.tab.c $(LDLIBS)

lex.yy.c: lexer.l
	flex $<

parser.tab.c parser.tab.h &: parser.y
	bison -d $<

all_clean: clean
	$(MAKE) all

clean:
	rm -f lex.yy.c parser.tab.c parser.tab.h PolyCalc
