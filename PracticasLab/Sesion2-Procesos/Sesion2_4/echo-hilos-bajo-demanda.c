// Implementación de un servidor que implementa  el servicio ECHO
// RCF 0862
// Se implementa solo el servicio bajo TCP. Se logra la concurrencia
// creando un hilo cada vez que llega un cliente. El hilo
// proporciona el servicio echo y el padre espera por otro cliente.
// Cuando el hilo finaliza de dar el servicio, termina.

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>


#define PUERTO 48666  // Puerto por defecto (el estándar 7 está protegido)
#define MAX_LINEA 80

/**** Definición de tipos de datos ****/
// Datos de la conexión (que se le pasan al hilo)
struct conex{
    char *origen;       //IP origen
    int puerto_origen;  //Puerto de origen
    int sock;           //Socket de conexión con el cliente
    unsigned int numhilo;  //Ordinal del hilo que atiende la petición
};
typedef struct conex datos_conexion;
/*************************************/

//Socket de servicio y socket de respuesta
int sockEscucha, sockDatos;

/* Un manejador para cuando se pulse Ctrl-C en el servidor*/
void SalirBien(int s)
{
    printf("Interrupción desde teclado. Terminando servidor.\n");
    close(sockEscucha);
    exit(0);
}

// Funciones para simplificar el acceso a la API de sockets
// Se ocupan de todo el manejo de errores
// ********************************************************
int CrearSocketServidorTCP(int puerto)
{
    int ret;
    int sock;
    struct sockaddr_in dir;

    ret=socket(PF_INET, SOCK_STREAM, 0);
    if (ret==-1) {
        perror("Al crear socket");
        exit(-1);
    }
    sock=ret;
    dir.sin_family=AF_INET;
    dir.sin_port=htons(puerto);
    dir.sin_addr.s_addr=htonl(INADDR_ANY);
    ret=bind(sock,(struct sockaddr *)  &dir, sizeof(dir));
    if (ret==-1) {
        perror("Al asignar direccion");
        close(sock);
        exit(-1);
    }
    ret=listen(sock, SOMAXCONN);
    if (ret==-1) {
        perror("Al poner en modo escucha");
        close(sock);
        exit(-1);
    }
    return sock;
}

int AceptarConexion(int sock, char **ip, int *puerto)
{
    int ret;
    struct sockaddr_in dir;
    static socklen_t longitud=sizeof(dir);

    ret=accept(sock, (struct sockaddr *) &dir, &longitud);
    if (ret==-1) {
        perror("Al aceptar");
        exit(-1);
    }
    *ip=inet_ntoa(dir.sin_addr);
    *puerto=ntohs(dir.sin_port);
    return ret;
}

int Enviar(int sock, char *buff, int longitud)
{
    int ret;
    ret=send(sock, buff, longitud, 0);
    if (ret==-1) {
        perror("Al enviar");
        exit(-1);
    }
    return ret;
}

int Recibir(int sock, char *buff, int longitud)
{
    int ret;
    ret=recv(sock, buff, longitud, 0);
    if (ret==-1) {
        perror("Al recibir");
        exit(-1);
    }
    return ret;
}

void CerrarSocket(int sock)
{
    if (close(sock)==-1) {
        perror("Al cerrar el socket");
    }
}
// ********************************************************

//Función que ejecuta el hilo que atiende cada petición
void * servicioEcho(datos_conexion *s)
{
    int recibidos;
    char buffer[MAX_LINEA];
    printf("Hilo %d: Recibida conexión desde %s(%d)\n",
            s->numhilo, s->origen, s->puerto_origen);
    //Leemos datos del socket mientras que el cliente no cierre la conexión
    do {
        recibidos=Recibir(s->sock, buffer, MAX_LINEA);
        printf("Hilo %d: Recibida un mensaje de longitud %d\n",
            s->numhilo, recibidos);
        buffer[recibidos]=0;  // Añadir terminador
        printf("Hilo %d: Contenido: %s\n", s->numhilo, buffer);
        Enviar(s->sock, buffer, strlen(buffer));
    } while (recibidos!=0);
    printf("Hilo %d: El cliente ha cerrado. Muero.\n", s->numhilo);
    CerrarSocket(s->sock);
    //Liberamos la memoria reservada para pasar los datos de la conexión
    //al hilo
    free(s->origen);
    free(s);
    return NULL; // El hilo debe retornar un puntero al terminar
}

int main(int argc, char *argv[])
{
    int puerto;
    char *desde;
    int puerto_desde;
    int n_hilos=0;
    pthread_t tid;
    datos_conexion *con;

    if (argc<2) {
        printf("Uso: %s [puerto]\n", argv[0]);
        printf("     puerto por defecto = % d\n", PUERTO);
    }

    if (argc>1) puerto=atoi(argv[1]);
    else puerto=PUERTO;

    // Tratar el Ctrl-C para cerrar bien el socket de escucha
    signal(SIGINT, SalirBien);

    sockEscucha=CrearSocketServidorTCP(puerto);
    while (1) {
        //Esperamos la llegada de nuevas conexiones
        sockDatos=AceptarConexion(sockEscucha, &desde, &puerto_desde);
        //Reservamos espacio para almacenar los datos de la nueva conexión
        con=(datos_conexion *) malloc(sizeof(datos_conexion));
        con->origen=(char *) malloc(strlen(desde)+1);
        strcpy(con->origen,desde);
        con->puerto_origen=puerto_desde;
        con->sock=sockDatos;
        con->numhilo=n_hilos;
        //Creamos un nuevo hilo que atenderá la nueva conexión
        pthread_create(&tid,NULL,(void *) servicioEcho, (void *) con);
        //Hacemos que el hilo sea independiente
        pthread_detach(tid);
        printf("Servidor principal: Lanzado el hijo %d\n", n_hilos);
        //Incrementamos el contador de hilos lanzados
        n_hilos++;
    }
    return 0;  // Nunca se alcanza, pero evita warning al compilar
}