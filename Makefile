CC = gcc
CFLAGS = -Wall -g

COMPILER_NAME := $(shell $(CC) --version)

ifneq (,$(findstring clang,$(COMPILER_NAME)))
    TYPE = CLANG
    CFLAGS += -Weverything
else ifneq (,$(findstring gcc,$(COMPILER_NAME)))
    TYPE = GCC
    CFLAGS += -Wextra
else
    TYPE = UNKNOWN
endif

all:
	@echo "Detectado: $(TYPE)"
	$(CC) $(CFLAGS) simulador.c -o simulador

all: programa

programa: simulador.o
	$(CC) $(CFLAGS) -o simulador simulador.o

simulador.o: simulador.c
	$(CC) $(CFLAGS) -c simulador.c

clean:
	rm -f *.o simulador programa