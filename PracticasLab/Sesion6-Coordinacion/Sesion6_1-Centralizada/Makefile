CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99

all: filosofos maestro

filosofos: filosofos.o
    $(CC) $(CFLAGS) -o filosofos filosofos.o

maestro: maestro.o
    $(CC) $(CFLAGS) -o maestro maestro.o

filosofos.o: filosofos.c filosofos.h
    $(CC) $(CFLAGS) -c filosofos.c

maestro.o: maestro.c maestro.h
    $(CC) $(CFLAGS) -c maestro.c

clean:
    rm -f filosofos maestro *.o