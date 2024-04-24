CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: filosofos maestro

filosofos: filosofos.o
	$(CC) $(CFLAGS) -o filosofos filosofos.o
	rm -f filosofos.o

maestro: maestro.o
	$(CC) $(CFLAGS) -o maestro maestro.o
	rm -f maestro.o

filosofos.o: filosofos.c filosofos.h
	$(CC) $(CFLAGS) -c filosofos.c

maestro.o: maestro.c maestro.h
	$(CC) $(CFLAGS) -c maestro.c

clean:
	rm -f filosofos maestro  # Eliminar solo los ejecutables