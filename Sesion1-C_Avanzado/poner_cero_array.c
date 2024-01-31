#include <stdio.h>

#define TERM -0xFFFF

void mostrar_array(int *p)
{
   int i = 0;
   while(*(p+i) != TERM)
   {
       printf("[%d]: %d\n", i, *(p+i));  // O podríamos poner p[i]
        i++;
   }
}

void poner_ceros(int *p)
{
    int i = 0;
   while(*(p+i) != TERM)
   {
        *(p+i) = 0;    // También vale p[i] = 0
        i++;
   }
}

int main()
{
    int a[] = {1,2,122,4,5, TERM};

    printf("ANTES:\n");
    mostrar_array(a);
    poner_ceros(a);
    printf("DESPUÉS:\n");
    mostrar_array(a);
    return 0;
}