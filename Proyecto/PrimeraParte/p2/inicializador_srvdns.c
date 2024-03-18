/*
Cliente de RPC que inicializa el servidor de srvdns
*/
#include <rpc/rpc.h>
#include <sys/types.h>
#include <sys/times.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include "srvdns.h"
#include "util.h"

// Prototipos de funciones
Lista *obtener_lista_dominios(char *fichrecords);
Lista *obtener_lista_tiposregistros(char *fichrecords);
int posicion_en_lista(char *cadena, Lista *lis);

int main(int argc, char *argv[])
{
    CLIENT *cl;
    char *ip_srvdns;
    FILE *fp;
    datini dat;

    if (argc != 3)
    {
        fprintf(stderr, "Forma de uso: %s <ip_srvdns> <fichero_registros>\n", argv[0]);
        exit(1);
    }
    ip_srvdns = strdup(argv[1]);
    if (!valida_ip(argv[1]))
    {
        fprintf(stderr, "Error: El parámetro IP no es valido\n");
        exit(2);
    }

    if ((fp = fopen(argv[2], "r")) == NULL)
    {
        fprintf(stderr, "Error: No se pudo abrir el fichero de registros\n");
        exit(3);
    }
    fclose(fp);

    // Inicializar la estructura CLIENT para las llamadas a procedimientos remotos
    // A RELLENAR
    |
    |
    |
    |
    |

    // Inicializar la estructura dat con las listas de dominios y tipos de registros
    dat.nomdominios = obtener_lista_dominios(argv[2]);
    dat.nomtiporecords = obtener_lista_tiposregistros(argv[2]);

    // Invocar el procedimiento remoto que inicializa el servidor en base a estas listas
    inicializar_srvdns_1(&dat, cl);
    clnt_destroy(cl);
    return 0;
}

// ******************************************************************
// Funciones de apoyo para la creación de las listas enlazadas
// ******************************************************************

Lista *obtener_lista_dominios(char *fichrecords)
{
    char buffer[TAMLINEA];
    FILE *fp;
    Lista *inicio = NULL;
    Lista *fin = NULL;
    Lista *p = NULL;
    char *token;

    if ((fp = fopen(fichrecords, "r")) == NULL)
    {
        fprintf(stderr, "Error: No se pudo abrir el fichero de registros\n");
        exit(3);
    }

    // Repetir para cada línea del fichero
    while ((fgets(buffer, TAMLINEA, fp)) != NULL)
    {
        buffer[strlen(buffer) - 1] = 0; // Quitar el salto de línea
        // extraemos el primer token de la linea (que es el nombre de dominio)
        token = strtok(buffer, ", \n");

        // Lo buscamos en la lista de los que ya hemos leido
        // Si ya estaba, lo saltamos
        if (posicion_en_lista(token, inicio) != NOENCONTRADO)
            continue;

        // En caso contrario, debe crearse un nuevo elemento para este nombre
        // de dominio y añadirlo a la lista (nota: si inicio==NULL es el primero
        // que se añade, y si no se usa el puntero fin para añadirlo al final)
        p = (Lista *)malloc(sizeof(Lista));
        if (p == NULL)
        {
            fprintf(stderr, "Error: No hay memoria suficiente para añadir un nuevo elemento a la lista de dominios\n");
            exit(4);
        }
        if (inicio == NULL) // la lista está vacía, añadimos el primer elemento
        {
            p->dato = strdup(token);
            p->siguiente = NULL;
            inicio = fin = p;
        }
        else // la lista ya tenía elementos
        {
            p->dato = strdup(token);
            p->siguiente = NULL;
            fin->siguiente = p;
            fin = p;
        }
    }
    fclose(fp);
    return (inicio);
}

Lista *obtener_lista_tiposregistros(char *fichrecords)
{
    char buffer[TAMLINEA];
    FILE *fp;
    Lista *inicio = NULL;
    Lista *fin = NULL;
    Lista *p = NULL;
    char *token;

    if ((fp = fopen(fichrecords, "r")) == NULL)
    {
        fprintf(stderr, "Error: No se pudo abrir el fichero de registros\n");
        exit(3);
    }
    while ((fgets(buffer, TAMLINEA, fp)) != NULL)
    {
        buffer[strlen(buffer) - 1] = 0;
        // extraemos el primer token de la linea
        token = strtok(buffer, ", \n"); // Pero no nos interesa el primer token
        token = strtok(NULL, ", \n");   // Nos interesa el segundo token

        // Hay que añadir a la lista enlazada este segundo token, si no estaba ya

        // Si ya estaba en la lista, saltamos esta iteración
        // A RELLENAR
        |
        |
        |


        // En caso contrario, debe crearse un nuevo elemento para este nombre
        // de dominio y añadirlo a la lista (nota: si inicio==NULL es el primero
        // que se añade, y si no se usa el puntero fin para añadirlo al final)
        p = (Lista *)malloc(sizeof(Lista));
        if (p == NULL)
        {
            fprintf(stderr, "Error: No hay memoria suficiente para añadir un nuevo elemento a la lista de tipos de registro\n");
            exit(4);
        }
        if (inicio == NULL) // la lista está vacía, añadimos el primer elemento
        {
            p->dato = strdup(token);
            p->siguiente = NULL;
            inicio = fin = p;
        }
        else // la lista ya tenía elementos
        {
            p->dato = strdup(token);
            p->siguiente = NULL;
            fin->siguiente = p;
            fin = p;
        }
    }
    fclose(fp);
    return (inicio);
}
