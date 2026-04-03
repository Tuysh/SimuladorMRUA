CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = -lm

# Regla principal
all: simulador

# Regla para construir el ejecutable
simulador: codigo/simulador.c
	$(CC) $(CFLAGS) codigo/simulador.c -o simulador $(LIBS)

# Regla para limpiar archivos generados
clean:
	rm -f simulador