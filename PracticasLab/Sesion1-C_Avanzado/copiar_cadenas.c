#include <stdio.h>
#include <stdlib.h>
int main()
{
  char *mensaje1;
  char *mensaje2;
  int  i;           // Para bucles

  mensaje1 = "Hola mundo";
  mensaje2 = malloc(11);   // Necesitamos 11 bytes (por que?)
  i = 0;
  while (mensaje1[i]!=0) {   // Mientras no encontremos terminador
     mensaje2[i] = mensaje1[i];
     i++;
  }
  // No olvidar el terminador de mensaje2
  mensaje2[++i] = 0;

  printf("mensaje1 apunta a %p y contiene %s\n", mensaje1, mensaje1);
  printf("mensaje2 apunta a %p y contiene %s\n", mensaje2, mensaje2);
  free(mensaje2); // Una vez terminamos con la copia, liberamos el espacio
  return 0;
}