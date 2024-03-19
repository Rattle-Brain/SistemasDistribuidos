// Archivos de cabecera para manipulación de sockets
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <pthread.h>
#include <errno.h>
#include "util.h"

#define TAMLINEA 1024
#define SINASIGNAR -1
#define MAXHILOSCLIENTE 10

// tipo de datos que recibiran los hilos cliente
struct datos_hilo
{
    unsigned char id;
    char *nom_fichero_consultas;
    struct sockaddr *dserv;
};

typedef struct datos_hilo datos_hilo;

//
// VARIABLES GLOBALES
//

// IP del proceso srvdns
char *ip_srvdns;

// Puerto en el que espera el proceso srvdns
int puerto_srvdns;

// Numero de hilos lectores
int nhilos;

// Es o no orientado a conexion
unsigned char es_stream = CIERTO;

// nombre del fichero fuente de consultas
char *fich_consultas;

char *hilos_file_names[MAXHILOSCLIENTE] = {
    "sal00.dat",
    "sal01.dat",
    "sal02.dat",
    "sal03.dat",
    "sal04.dat",
    "sal05.dat",
    "sal06.dat",
    "sal07.dat",
    "sal08.dat",
    "sal09.dat"};

void procesa_argumentos(int argc, char *argv[])
{
    if (argc < 6)
    {
        fprintf(stderr, "Forma de uso: %s ip_srvdns puerto_srvdns {t|u} nhilos fich_consultas\n", argv[0]);
        exit(1);
    }
    // Verificación de los argumentos e inicialización de las correspondientes variables globales.
    // Puedes usar las funciones en util.h

    // A RELLENAR
    // Validamos puerto
    if(valida_numero(argv[2]))
    {
        puerto_srvdns = atoi(argv[2]);
        if(!puerto_en_rango(puerto_srvdns))
        {
            perror("Puerto no valido. Fuera de rango\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        perror("Puerto no valido. No numerico\n");
        exit(EXIT_FAILURE);
    }
}

void salir_bien(int s)
{
    exit(0);
}

void *hilo_lector(datos_hilo *p)
{
    int enviados, recibidos;
    char buffer[TAMLINEA];
    char respuesta[TAMLINEA];
    char *s;
    int sock_dat;
    FILE *fpin;
    FILE *fpout;

    if ((fpin = fopen(p->nom_fichero_consultas, "r")) == NULL)
    {
        perror("Error: No se pudo abrir el fichero de consultas");
        pthread_exit(NULL);
    }
    if ((fpout = fopen(hilos_file_names[p->id], "w")) == NULL)
    {
        fclose(fpin); // cerramos el handler del fichero de consultas
        perror("Error: No se pudo abrir el fichero de resultados");
        pthread_exit(NULL);
    }
    do
    {
        bzero(buffer, TAMLINEA);
        s = fgets(buffer, TAMLINEA, fpin);

        if (s != NULL)
        {
            if (es_stream)
            {
                // Enviar el mensaje leído del fichero a través de un socket TCP
                // y leer la respuesta del servidor
                // A RELLENAR
                |
                |
                |
                |
            }
            else
            {
                // Enviar el mensaje leído del fichero a través de un socket UDP
                // y leer la respuesta del servidor
                // A RELLENAR
                |
                |
                |
                |
            }
            close(sock_dat);
            // Volcar la petición y la respuesta, separadas por ":" en
            // el fichero de resultados
            // A RELLENAR
            |
            |
        }
    } while (s);
    // Terminado el hilo, liberamos la memoria del puntero y cerramos ficheros
    fclose(fpin);
    fclose(fpout);
    free(p);
    return NULL;
}

int main(int argc, char *argv[])
{
    register int i;

    pthread_t *th;
    datos_hilo *q;

    struct sockaddr_in d_serv;

    // handler de archivo
    FILE *fp;

    signal(SIGINT, salir_bien);
    procesa_argumentos(argc, argv);

    // Comprobar si se puede abrir el fichero, para evitar errores en los hilos
    if ((fp = fopen(fich_consultas, "r")) == NULL)
    {
        perror("Error: No se pudo abrir el fichero de consultas");
        exit(6);
    }
    else
        fclose(fp); // cada hilo lo abrirá para procesarlo

    if ((th = (pthread_t *)malloc(sizeof(pthread_t) * nhilos)) == NULL)
    {
        fprintf(stderr, "No se pudo reservar memoria para los objetos de datos de hilo\n");
        exit(7);
    }

    // Inicializar la estructura de dirección del servidor que se pasará a los hilos
    // A RELLENAR
    |
    |
    |

    for (i = 0; i < nhilos; i++)
    {
        // Preparar el puntero con los parámetros a pasar al hilo
        // A RELLENAR
        q = // 
        |
        |
        |
        // Crear el hilo
        // A RELLENAR
        if ( ??? != 0)
        {
            fprintf(stderr, "No se pudo crear el hilo lector %d\n", i);
            exit(9);
        }
    }
    
    // Esperar a que terminen todos los hilos
    for (i = 0; i < nhilos; i++)
    {
        pthread_join(th[i], NULL);
    }
}
