#include <stdio.h>

int main()
{
    int a[] = {1,2,3,4,5};
    int *p;

    p = a;
    printf("El primer elemento del array vale %d\n", *p);
    // Lo modificamos
    *p = 8;
    // Y en consecuencia el array ha cambiado también
    printf("Ahora vale %d\n", a[0]);
    return 0;
}