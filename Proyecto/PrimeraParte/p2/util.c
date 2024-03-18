#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "srvdns.h"
#include "util.h"

// Función de utilidad que determina si los caracteres de una cadena
// son todos numericos
int valida_numero(char *str)
{
    while (*str)
    {
        if (!isdigit(*str))
        { // si el caracter no es un numero retorna falso
            return FALSE;
        }
        str++; // apuntamos al siguiente caracter
    }
    return CIERTO;
}

// Función de utilidad que valida si una cadena de caracteres representa
// una IPv4 valida
int valida_ip(char *ip)
{ // comprueba si la IP es valida o no
    int num, dots = 0;
    char *mip;
    char *ptr;

    if (ip == NULL)
        return FALSE;
    mip = strdup(ip);       // para no modificar la ip original
    ptr = strtok(mip, "."); // extrae los tokens de la cadena delimitados por '.'
    if (ptr == NULL)
        return FALSE;
    while (ptr)
    {
        if (!valida_numero(ptr)) // comprueba si la subcadena es un numero o no
            return FALSE;
        num = atoi(ptr); // convierte la subcadena a entero
        if (num >= 0 && num <= 255)
        {
            ptr = strtok(NULL, "."); // extrae la siguiente subcadena
            if (ptr != NULL)
                dots++; // incrementa el contador de delimitadores
        }
        else
            return FALSE;
    }
    if (dots != 3) // si el numero de '.' es distinto de 3, retorna falso
        return FALSE;
    return CIERTO;
}

// Función de utilidad, para generar los tiempos usados por los buques en las operaciones portuarias
// Devuelve un número aleatorio comprendido entre min y max
double randRange(double min, double max)
{
    return min + (rand() / (double)RAND_MAX * (max - min + 1));
}

// Función de utilidad para depuración. Emite por pantalla el mensaje
// que se le pasa como parámetro, pero pone delante del mensaje un
// timestamp, para poder ordenar la salida por si saliera desordenada
//
// Ejemplo de uso:
//
//  log_debug("Mensaje a imprimir")
//
// Más ejemplos en el programa principal.
void log_debug(char *msg)
{
    struct timespec t;
    clock_gettime(_POSIX_MONOTONIC_CLOCK, &t);
    printf("[%ld.%09ld] %s", t.tv_sec, t.tv_nsec, msg);
}

void mostrar_recuento_consultas(int nfils, int ncols, Lista *nomdominios, Lista *nomtiposrec, int **valores)
{
    register int i, j;
    Lista *p;

    fprintf(stderr, "*****************  RECUENTO CONSULTAS  *******************\n");

    fprintf(stderr, "\t\t");
    p = nomtiposrec;
    while (p)
    {
        fprintf(stderr, "%s\t", p->dato);
        p = p->siguiente;
    }
    fprintf(stderr, "\n");
    p = nomdominios;
    i = 0;
    while (p)
    {
        fprintf(stderr, "%s\t", p->dato);

        for (j = 0; j < ncols; j++)
            fprintf(stderr, "%d\t", valores[i][j]);

        fprintf(stderr, "\n");
        i++;
        p = p->siguiente;
    }
}


// Función de utilidad para ver si una cadena tiene el valor MX o NS
int es_MX_o_NS(char *tiporecord)
{
    if ((strcmp(tiporecord, "MX") == 0) || (strcmp(tiporecord, "NS") == 0))
    {
        return CIERTO;
    }
    return FALSO;
}

// **********************************************************************
// Funciones auxiliares para el manejo de las listas enlazadas
// Son útiles tanto para los clientes como para el servidor
// **********************************************************************

int obtener_longitud_lista(Lista *p)
{
    int n = 0;

    while (p)
    {
        n++;
        p = p->siguiente;
    }
    return (n);
}

char *obtener_dato_en_posicion(int n, Lista *p)
{
    // Avanza por la lista, elemento a elemento, hasta llegar al que
    // ocupa la posición n, y retorna una copia (strdup) de su dato,
    // o retorna NULL si termina la lista antes de llegar a la posición n.

    char *cad = NULL;

    // A RELLENAR
    |
    |
    |
    |
    |
    |
    
    return (cad);
}

int posicion_en_lista(char *cad, Lista *p)
{
    // Busca la cadena dada en la lista y retorna la posición en la que se encuentra
    // o la constante NOENCONTRADO si no está.

    int pos = NOENCONTRADO;

    // A RELLENAR
    |
    |
    |
    |
    |
    |
    
    return (pos);
}