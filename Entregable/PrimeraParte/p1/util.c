#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include "util.h"

// Función de utilidad que determina si los caracteres de una cadena
// son todos numericos
int valida_numero(char *str)
{
    while (*str)
    {
        if (!isdigit(*str))
        { // si el caracter no es un numero retorna falso
            return FALSO;
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
    char *ptr;
    if (ip == NULL)
        return FALSO;
    ptr = strtok(ip, "."); // extrae los tokens de la cadena delimitados por '.'
    if (ptr == NULL)
        return FALSO;
    while (ptr)
    {
        if (!valida_numero(ptr)) // comprueba si la subcadena es un numero o no
            return FALSO;
        num = atoi(ptr); // convierte la subcadena a entero
        if (num >= 0 && num <= 255)
        {
            ptr = strtok(NULL, "."); // extrae la siguiente subcadena
            if (ptr != NULL)
                dots++; // incrementa el contador de delimitadores
        }
        else
            return FALSO;
    }
    if (dots != 3) // si el numero de '.' es distinto de 3, retorna falso
        return FALSO;
    return CIERTO;
}

// Función de utilidad, para generar los tiempos aleatorios entre un
// min y un max
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
//  log_debug("Mensaje a mostrar por pantalla")
//
void log_debug(char *msg)
{
    struct timespec t;
    clock_gettime(_POSIX_MONOTONIC_CLOCK, &t);

    printf("[%ld.%09ld] %s", t.tv_sec, t.tv_nsec, msg);
}
