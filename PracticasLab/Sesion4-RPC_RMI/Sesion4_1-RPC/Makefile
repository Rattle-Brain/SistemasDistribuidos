LDFLAGS=-ltirpc
CC=gcc
CFLAGS=-I/usr/include/tirpc

all: cliente servidor

cliente: cliente1.o calculadora_xdr.o calculadora_clnt.o
	gcc -o $@ $^ $(LDFLAGS)

servidor: servicios.o calculadora_xdr.o calculadora_svc.o
	gcc -o $@ $^ $(LDFLAGS)

cliente.o: cliente1.c calculadora.h
	gcc -o $@ $(LDFLAGS) $^

servidor.o: servidor.c calculadora.h
	gcc -o $@ $(LDFLAGS) $^

calculadora.h calculadora_xdr.c calculadora_svc.c calculadora_clnt.c: calculadora.x
	rpcgen $<

clean:
	rm *.o calculadora_*.c calculadora.h  
