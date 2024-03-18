# EPI (Gijon)
# Grado en Ingenieria Informatica
# Sistemas de Distribuidos (Curso 3)
# Aplicaciones Distribuidas en Linux/Unix con Sun RPC


#****************************************************
# Variables
#****************************************************


CC=gcc
RPCGEN=rpcgen
CFLAGS= -I. -Wall -ggdb
LDFLAGS= -pthread
CLIENTE = cliente
ESTADIS = estadis
SERVIDOR= servidor
INICIALIZADOR= inicializador_srvdns

todo : $(CLIENTE) $(SERVIDOR) $(INICIALIZADOR) $(ESTADIS)

#**************************************************
# Generacion del cliente y del servidor
#**************************************************

$(CLIENTE) : cliente.o srvdns_clnt.o srvdns_xdr.o util.o
	$(CC) $(CFLAGS) -o $(CLIENTE) $(LDFLAGS) cliente.o srvdns_clnt.o srvdns_xdr.o util.o

$(ESTADIS) : estadis.o srvdns_clnt.o srvdns_xdr.o util.o
	$(CC) $(CFLAGS) -o $(ESTADIS) $(LDFLAGS) estadis.o srvdns_clnt.o srvdns_xdr.o util.o

$(INICIALIZADOR) : inicializador_srvdns.o srvdns_clnt.o srvdns_xdr.o util.o
	$(CC) $(CFLAGS) -o $(INICIALIZADOR) $(LDFLAGS) inicializador_srvdns.o srvdns_clnt.o srvdns_xdr.o util.o

$(SERVIDOR) : srvdns_svc.o srvdns_xdr.o util.o servicios.o
	$(CC) $(CFLAGS) -o $(SERVIDOR) srvdns_svc.o srvdns_xdr.o util.o servicios.o

srvdns_svc.o : srvdns_svc.c srvdns.h
	$(CC) $(CFLAGS) -c srvdns_svc.c 

servicios.o: servicios.c srvdns.h util.h
	$(CC) $(CFLAGS) -c servicios.c

srvdns_clnt.o : srvdns_clnt.c srvdns.h
	$(CC) $(CFLAGS) -c srvdns_clnt.c

srvdns_xdr.o : srvdns_xdr.c srvdns.h
	$(CC) $(CFLAGS) -c srvdns_xdr.c

cliente.o : cliente.c srvdns.h util.h 
	$(CC) $(CFLAGS) -c cliente.c

estadis.o : estadis.c srvdns.h util.h 
	$(CC) $(CFLAGS) -c estadis.c

inicializador_srvdns.o : inicializador_srvdns.c util.h srvdns.h
	$(CC) $(CFLAGS) -c inicializador_srvdns.c 

util.o : util.c util.h srvdns.h
	$(CC) $(CFLAGS) -c util.c

srvdns.h: srvdns.x
	$(RPCGEN) $<

#**************************************************
# Utilidades
#**************************************************

limpia : 
	rm -f *.o	

cleanall: limpia
	rm -f srvdns_*
	rm -f srvdns.h
	rm -f sal*.dat
	rm -f cliente servidor inicializador_srvdns estadis
