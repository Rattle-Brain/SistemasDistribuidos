#include <stdio.h>

int main()
{
    char   c = 'a';
    int    i = 500;
    double z = 0.0;

    // Punteros a las variables anteriores
    char   *p_c = &c;
    int    *p_i = &i;
    double *p_z = &z;

    printf("Tamaño de c: %lu\n", sizeof(c));
    printf("Tamaño de i: %lu\n", sizeof(i));
    printf("Tamaño de z: %lu\n", sizeof(z));
    printf("Puntero p_c: %p\n", p_c);
    printf("Puntero p_i: %p\n", p_i);
    printf("Puntero p_z: %p\n", p_z);
    printf("Tamaño del puntero p_c: %lu\n", sizeof(p_c));
    printf("Tamaño del puntero p_i: %lu\n", sizeof(p_i));
    printf("Tamaño del puntero p_z: %lu\n", sizeof(p_z));

    int j = 3;
    printf ("%d\n", (*p_i)*j);
}