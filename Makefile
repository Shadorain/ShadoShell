# Shado Shell Makefile
CC=gcc
CFLAGS=-lreadline -g #-I. -g
#DEPS=config.h
AM_YFLAGS = -d
OBJ=shadosh.o node.c nalloc.c exec.c y.tab.c lex.yy.c
BDIR=/usr/local

#%.o: %.c $(DEPS)
# 	$(CC) -c -o $@ $< $(CFLAGS)

shadosh: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

lex.yy.c: y.tab.c parse.l
	lex parse.l

y.tab.c: parse.y
	yacc -d parse.y

install:
	mkdir -p $(BDIR)/bin
	cp -f shadosh $(BDIR)/bin
	chmod 755 $(BDIR)/bin/shadosh

clean:
	rm -f *.o shadosh

delobj:
	rm -f *.o

uninstall:
	rm -f $(BDIR)/bin/shadosh

.PHONY: clean install uninstall
