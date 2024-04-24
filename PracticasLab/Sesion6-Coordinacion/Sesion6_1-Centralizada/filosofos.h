#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Funciones para simplificar la API de sockets. Se declaran aquí
// pero no se implementan. El alumno puede copiar su implementación
// de otros ejercicios, o implementarlas él mismo

/*
*****************************************************
*  NO TE AGOBIES PAPI QUE LAS TIENES IMPLEMENTADAS  *
*****************************************************
*/

// Crea un socket y si no hay errores lo retorna
int CrearSocketClienteTCP(void);

// Conecta a la IP y puertos dados. La IP se le pasa como cadena
// Si hay errores termina el programa
void ConectarConServidor(int s, char *ip, int puerto);

// Las dos siguientes funciones equivalen a send() y recv() pero
// si detectan un error terminan el programa
int Enviar(int s, char *buff, int longitud);
int Recibir(int s, char *buff, int longitud);

// Cierra el socket
void CerrarSocket(int s);

#define MAX_BOCADOS 10
#define LOCK 1
#define UNLOCK 2

//Prototipos de las funciones
void filosofo(int numfilo,char *ip,int puerto,int maxfilo);
int controlMutex(char *ipmaster, int puerto, char op, int nummutex);