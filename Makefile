# Shado Shell Makefile
CC=gcc
CFLAGS=-lreadline #-I. -g
#DEPS=config.h
OBJ=shadosh.o parsing.o exec.o builtins.o
BDIR=/usr/local

#%.o: %.c $(DEPS)
# 	$(CC) -c -o $@ $< $(CFLAGS)

shadosh: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

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
