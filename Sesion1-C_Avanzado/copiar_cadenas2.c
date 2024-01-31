#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
  char *mensaje1;
  char *mensaje2;

  mensaje1 = "Hola mundo";
  mensaje2 = strdup(mensaje1);

  printf("mensaje1 apunta a %p y contiene %s\n", mensaje1, mensaje1);
  printf("mensaje2 apunta a %p y contiene %s\n", mensaje2, mensaje2);
  free(mensaje2); // Una vez terminamos con la copia, liberamos el espacio
  return 0;
}