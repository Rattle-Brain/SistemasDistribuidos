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
    // Validamos IP

    // Hacemos copia de IP para evitar posibles problemas

    char * ip_serv_dup = strdup(argv[1]);


    if(valida_ip(ip_serv_dup))
    {
        ip_srvdns = argv[1];
    }
    else 
    {
        perror("IP no válida. Formato XXX.XXX.XXX.XXX donde XXX = [0-255].\n");
        exit(EXIT_FAILURE);
    }

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

    // Comprobamos el protocolo
    if(strcmp(argv[3], "u") == 0)
    {
        es_stream = FALSO;
    }
    else if (strcmp(argv[3], "t") != 0 && strcmp(argv[3], "u") != 0) 
    {
        perror("Protocolo invalido.\n\tt - TCP\n\tu - UDP\n");
        exit(EXIT_FAILURE);
    }

    // Comprobamos el numero de hilos de atencion
    if(valida_numero(argv[4]) && atoi(argv[4]) <= MAXHILOSCLIENTE && atoi(argv[4]) >= 1)
    {
        nhilos = atoi(argv[4]);
    }
    else 
    {
        fprintf(stderr, "Numero de hilos no valido. MIN 0 - MAX %d\n", MAXHILOSCLIENTE);
        exit(EXIT_FAILURE);
    }

    fich_consultas = argv[5];
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

                /**
                    Por alguna razón, si aqui utilizo p->dserv en lugar de crear un struct sockaddr_in nuevo, me da un error
                    en la funcion CONNECT:
                        Error al conectar al servidor TCP: Address family not supported by protocol
                    
                    Sin embargo, cuando utilizo este sockaddr_in creado aquí funciona bien. En UDP no tengo este problema y 
                    si que puedo utilizar p->dserv sin problemas. No tengo idea de por que.

                    No obstante, y como no fui capaz de resolver el probelma, 
                    para que el programa funcione decidi dejar un socket creado aqui.
                */
                struct sockaddr_in srv_addr;
                srv_addr.sin_family = AF_INET;
                srv_addr.sin_port = htons(puerto_srvdns);
                srv_addr.sin_addr.s_addr = inet_addr(ip_srvdns);

                sock_dat = socket(PF_INET, SOCK_STREAM, 0);
                if (sock_dat < 0)
                {
                    perror("Error al crear el socket TCP");
                    fclose(fpin);
                    fclose(fpout);
                    pthread_exit(NULL);
                }

                // Conectar al servidor
                if (connect(sock_dat, (struct sockaddr * )&srv_addr, sizeof(srv_addr)) < 0)
                {
                    perror("Error al conectar al servidor TCP");
                    fclose(fpin);
                    fclose(fpout);
                    close(sock_dat);
                    pthread_exit(NULL);
                }

                // Enviamos la consulta al servidor
                enviados = send(sock_dat, buffer, strlen(buffer), 0);
                if (enviados < 0)
                {
                    perror("Error al enviar el mensaje TCP al servidor");
                    fclose(fpin);
                    fclose(fpout);
                    close(sock_dat);
                    pthread_exit(NULL);
                }

                // Recibimos la respuesta del servidor
                recibidos = recv(sock_dat, respuesta, sizeof(respuesta), 0);
                if (recibidos < 0)
                {
                    perror("Error al recibir la respuesta TCP del servidor");
                    fclose(fpin);
                    fclose(fpout);
                    close(sock_dat);
                    pthread_exit(NULL);
                }
            }
            else
            {
                // Enviar el mensaje leído del fichero a través de un socket UDP
                // y leer la respuesta del servidor
                // A RELLENAR
                
                // Enviar el mensaje leído del fichero a través de un socket UDP
                // y leer la respuesta del servidor
                sock_dat = socket(PF_INET, SOCK_DGRAM, 0);
                if (sock_dat < 0)
                {
                    perror("Error al crear el socket UDP");
                    fclose(fpin);
                    fclose(fpout);
                    pthread_exit(NULL);
                }

                // Enviamos la consulta al servidor
                enviados = sendto(sock_dat, buffer, strlen(buffer), 0, (struct sockaddr *)(p->dserv), sizeof(*p->dserv));
                if (enviados < 0)
                {
                    perror("Error al enviar el mensaje UDP al servidor");
                    fclose(fpin);
                    fclose(fpout);
                    close(sock_dat);
                    pthread_exit(NULL);
                }

                // Recibimos la respuesta del servidor
                recibidos = recvfrom(sock_dat, respuesta, sizeof(respuesta), 0, NULL, NULL);
                if (recibidos < 0)
                {
                    perror("Error al recibir la respuesta UDP del servidor");
                    fclose(fpin);
                    fclose(fpout);
                    close(sock_dat);
                    pthread_exit(NULL);
                }
            }
            close(sock_dat);
            // Volcar la petición y la respuesta, separadas por ":" en
            // el fichero de resultados
            // A RELLENAR

            // Añadimos el carácter nulo al final de la respuesta
            respuesta[recibidos] = 0;
            respuesta[strlen(respuesta) - 1] = 0; // Quitar el retorno de carro

            // Añadir el terminador de cadena
            buffer[recibidos] = 0;          
            buffer[strlen(buffer) - 1] = 0; 
            fprintf(fpout, "%s:%s\n", buffer, respuesta);
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
    d_serv.sin_family = AF_INET;
    d_serv.sin_port = htons(puerto_srvdns);
    inet_aton(ip_srvdns, &d_serv.sin_addr);

    for (i = 0; i < nhilos; i++)
    {
        // Preparar el puntero con los parámetros a pasar al hilo
        // A RELLENAR
        q = (datos_hilo*)malloc(sizeof(datos_hilo));
        q->id = i;
        q->nom_fichero_consultas = fich_consultas;
        q->dserv = (struct sockaddr *)&d_serv;

        // Crear el hilo
        // A RELLENAR
        if (pthread_create(&th[i], NULL, (void*)hilo_lector, (void*)q) != 0)
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
