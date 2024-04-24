/**
    FICHERO DE CABECERAS PARA FILODIST.C

    MODIFICADO LIGERAMENTE PORQUE NO ME GUSTABA 
    COMO ESTABA HECHO
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define MAX_BOCADOS 10

typedef enum {
     no_sentado=0, queriendo_comer=1,
     comiendo=2, dejando_comer=3,
     pensando=4} estado_filosofo;

/* prototipos funciones*/
void procesaLineaComandos(int numero,char *lista[]);
void inicializaciones(void);
void * filosofo(void);
void esperarPalillos(void);
void soltarPalillos(void);
void cambiarEstado(estado_filosofo nuevoestado);
char palillosLibres(unsigned char token);
void alterarToken(unsigned char *tok, estado_filosofo nuevoestado);
void * comunicaciones(void);
void * esperarConexion(void);