#include <stdio.h>

struct Complejo {
    double real;
    double imag;
};

int main()
{
    struct Complejo c;   // c es un dato de tipo Complejo
    struct Complejo *p;  // p es un puntero al tipo Complejo

    c.real = 5;
    c.imag = 10;

    printf("Con los valores del STRUCT:\n");
    printf("Parte real: %f. \nParte imaginaria %f\n", c.real, c.imag);
        
    p = &c;             // Hago que p apunte a la dirección de c

    printf("\nCon los valores del PUNTERO (sintaxis punto):\n");
    printf("Parte real: %f. \nParte imaginaria %f\n", (*p).real, (*p).imag);  // Desreferenciar el puntero accede a las propiedades

    printf("\nCon los valores del PUNTERO (sintaxis flecha):\n");
    printf("Parte real: %f. \nParte imaginaria %f\n", p->real,  p->imag);   // Si no desreferenciamos, podemos usar la flecha
}