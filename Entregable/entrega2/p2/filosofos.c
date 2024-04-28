/*****************************************************************************

                               filosofos.c

En esta version distribuida del problema de los filosofos requiere de un
maestro que controle los recursos compartidos (palillos).

*****************************************************************************/
#include <stdio.h>      // FASE 2
#include <time.h>       // FASE 2
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

#define LOG_MSG_SIZE 50     // FASE 2: Define tamaño de la variable log_msg.

//Prototipos de las funciones
void filosofo(int numfilo,char *ip,int puerto,int maxfilo);
int controlMutex(char *ipmaster, int puerto, char op, int palillo1, int palillo2);    // FASE 3: Modificacion del protocolo

// FASE 2:
void log_debug(char* msg); 

char * log_msg = NULL;      // FASE 2: Declaracion de la variable necesaria

int main(int argc, char *argv[])
{
    int numfilo;
    int puerto;
    int maxfilo;

    setbuf(stdout, NULL);        // FASE 2: Cancelacion de la buferizacion
    log_msg = malloc(sizeof(char) * LOG_MSG_SIZE);

    //Comprobacion del número de argumentos
    if (argc<5)
    {
        fprintf(stderr,"Forma de uso: %s numfilo ip_maestro puerto maxfilo\n",
                argv[0]);
        exit(1);
    }
    /*** Lectura de valores de los argumentos ***/
    numfilo=atoi(argv[1]);
    if ((numfilo<0) || (numfilo>4))
    {
        fprintf(stderr,"El numero de filosofo debe ser >=0 y <=4\n");
        exit(2);
    }
    puerto=atoi(argv[3]);
    if ((puerto<0) || (puerto>65535))
    {
        fprintf(stderr,"El numero de puerto debe ser >=0 y <=65535\n");
        exit(3);
    }
    maxfilo=atoi(argv[4]);
    if (maxfilo<0)
    {
        fprintf(stderr,"El numero de filosofos debe ser mayor que 0\n");
        exit(3);
    }
    /*********************************************/
    //lanzamiento del filósofo
    filosofo(numfilo,argv[2],puerto,maxfilo);
    free (log_msg);     // FASE 2: Free del malloc
    exit(0);
}

//función que implementa las tareas a realizar por el filósofo
void filosofo(int numfilo,char *ip,int puerto,int maxfilo)
{
    int veces=0;

    // FASE 3: Extraccion de variables por limpieza.
    int palillo1 = numfilo;
    int palillo2 = (numfilo+1)%maxfilo;

    // FASE 2: log_debug
    sprintf(log_msg, "\033[1;34mFilosofo %d sentado a la mesa.\033[0m\n", numfilo);
    log_debug(log_msg);
    while (veces<MAX_BOCADOS)
    {
        // mientras el acuse de la solicitud de palillo derecho sea 0
        // seguimos intentándolo
        while (controlMutex(ip, puerto, LOCK, palillo1, palillo2)==0);     // FASE 3: Modificacion del protocolo
        // mientras el acuse de la solicitud de palillo izquierdo sea 0
        // seguimos intentándolo
        // while (controlMutex(ip,puerto,LOCK,palillo2)==0);
        // ya tenemos ambos palillos y por tanto podemos comer

        // FASE 2: log_debug
        sprintf(log_msg, "\033[1;31mEl filosofo %d toma palillos %d, %d y esta comiendo.\033[0m\n", numfilo, palillo1, palillo2);
        log_debug(log_msg);

        sleep(3);
        // mientras el acuse de liberación de palillo izquierdo sea 0
        // seguimos intentando
        //liberar el palillo
        while (controlMutex(ip, puerto, UNLOCK, palillo1, palillo2)==0);   // FASE 3: Modificacion del protocolo
        // mientras el acuse de liberación de palillo derecho sea 0
        // seguimos intentando
        // liberar el palillo
        //while (controlMutex(ip,puerto,UNLOCK,palillo1)==0);
        //el filosofo ha soltado ambos palillos y puede dedicarse a pensar

        // FASE 2: log_debug
        sprintf(log_msg, "\033[1;32mEl filosofo %d esta pensando.\033[0m\n", numfilo);
        log_debug(log_msg);

        sleep(5);
        // incrementamos el número de veces que el filósofo
        // ha podido completar el ciclo
        veces++;
    }
    //el filósofo ha completado el número de bocados y se levanta de la mesa

    // FASE 2: log_debug
    sprintf(log_msg, "\033[1;34mEl filosofo %d se ha levantado de la mesa.\033[0m\n", numfilo);
    log_debug(log_msg);
}

//esta función le permite al filósofo solicitar-liberar un recurso
//gestionado por el coordinador
int controlMutex(char *ipmaster, int puerto, char op, int palillo1, int palillo2)   // FASE 3: Modificacion del protocolo
{
    //Necesita de las funciones de libsokets
    int sock;
    char buffer[10];
    char *ptr;

    //crea el socket de conexion con el maestro de recursos
    sock=CrearSocketClienteTCP();
    //abre conexión con el maestro de recursos
    ConectarConServidor(sock, ipmaster, puerto);
    //en buffer confecciona el mensaje en función de la operación
    //a realizar y el palillo a solicitar (LOCK) o liberar (UNLOCK)
    sprintf(buffer,"%c",'\0');

    switch (op){
        case LOCK:
            sprintf(buffer,"L %d %d",palillo1, palillo2);       // FASE 3: Modificacion del protocolo
            break;
        case UNLOCK:
            sprintf(buffer,"U %d %d",palillo1, palillo2);       // FASE 3: Modificacion del protocolo
            break;
    }

    //se envía el mensaje al maestro de recursos
    Enviar(sock,buffer,strlen(buffer));
    //se inicializa el buffer para la recepción
    sprintf(buffer,"%c",'\0');
    //se espera síncronamente la respuesta del mensaje
    Recibir(sock,buffer,10);
    //se obtiene en ptr un puntero al primer token del mensaje
    ptr=strtok(buffer," \t\n");
    //se cierra el socket de comunicación con el servidor
    CerrarSocket(sock);
    //se retorna el valor entero del token recibido
    //1-OK, 0-NO OK
    return(atoi(ptr));
}

// FASE 1:
// Funciones que faltaban

// FASE 1:
// Devuelve el fd del socket TCP creado
int CrearSocketClienteTCP(void){
    int socketTCP;

    // Crear el socket TCP
    socketTCP = socket(AF_INET, SOCK_STREAM, 0);
    if (socketTCP == -1) {
        perror("Error al crear el socket TCP");
        return -1;
    }
    return socketTCP;
}

// FASE 1:
// Con los parametros que recibe trata de conectarse a un servidor
// Si falla pues para la ejecucion.
void ConectarConServidor(int s, char *ip, int puerto) {
    struct sockaddr_in servaddr;

    // Configurar la dirección del servidor
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip); // Convertir la dirección IP de cadena a formato numérico
    servaddr.sin_port = htons(puerto); // Convertir el puerto de host a orden de red

    // Conectar al servidor
    if (connect(s, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
        perror("Error al conectar con el servidor");
        close(s);
        exit(EXIT_FAILURE);
    }
}

// FASE 1:
// Función para cerrar un socket
void CerrarSocket(int s) {
    int resultado = close(s);
    if (resultado == -1) {
        perror("Error al cerrar el socket");
        exit(EXIT_FAILURE);
    }
}

// Función para enviar datos a través de un socket
// Si hay errores, termina el programa
int Enviar(int s, char *buff, int longitud) {
    int rec_bytes = send(s, buff, longitud, 0);
    if (rec_bytes == -1) {
        perror("Error al enviar datos");
        close(s);
        exit(EXIT_FAILURE);
    }
    return rec_bytes;
}

// FASE 1:
// Función para recibir datos a través de un socket
// Si hay errores, termina el programa
int Recibir(int s, char *buff, int longitud) {
    int rec_bytes = recv(s, buff, longitud, 0);
    if (rec_bytes == -1) {
        perror("Error al recibir datos");
        close(s);
        exit(EXIT_FAILURE);
    }
    return rec_bytes;
}

// FASE 2: log_debug definition
void log_debug(char *msg)
{
    struct timespec t;
    clock_gettime(_POSIX_MONOTONIC_CLOCK, &t);

    printf("[%ld.%09ld] %s", t.tv_sec, t.tv_nsec, msg);
}