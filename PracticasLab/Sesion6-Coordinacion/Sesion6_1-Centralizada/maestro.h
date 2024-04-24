// Implementación de un maestro de recursos para
// coordinacion distribuida
#include <sys/socket.h> // socket, send, recv...
#include <netinet/in.h> // sockaddr_in, htons, htonl, etc.
#include <arpa/inet.h>  // inet_addr
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>  //prototipos de funciones para mutex

#define MAX_LINEA 80 // Maxima longitud esperada para una lectura

//Estructura que se le va a pasar a cada hilo que atiende una petición
typedef struct {
    int puerto;
    pthread_mutex_t * lista;
    int tamlista;
}
petinfo;

// Esta función crea el socket de escucha, le asigna el número de
// puerto y le pone en modo "listen". Retorna el socket creado.
// El código de la función está más abajo (después del main())
int InicializarSocketEscucha(int puerto);

// Esta función hace una lectura del socket de datos.  Si recibe cero,
// retorna. En otro caso, lee del socket, interpreta el mensaje,
// hace la operacion correspondiente sobre la posicion del array de mutex
// y envia confirmacion
void OperarMutex(petinfo * pet, int socket_datos, int * lista, int listalen);

// Esta funcion inicializa cada posicion del array de mutex

void InicializaMutex(pthread_mutex_t * lista, int tam);

//Procesa una nueva peticion de acceso a un recurso
void * procesaPeticion(petinfo * p);