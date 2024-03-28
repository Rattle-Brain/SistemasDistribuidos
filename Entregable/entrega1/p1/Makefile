CC=gcc
CFLAGS=-I. -pthread -g
DEPS = cola.c util.c 
OBJ = srvdns.o cola.o cliente.o util.o

all: srvdns cliente

srvdns: srvdns.c $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS) 

cliente: cliente.c util.c util.h
	$(CC) -o $@ $^ $(CFLAGS) 

limpia: 
	rm *.o
cleanall:
	-rm srvdns cliente
	-rm sal*.dat
