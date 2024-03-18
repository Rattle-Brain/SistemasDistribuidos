#include <stdio.h>
int main() {
  int dato;
  int *p = &dato; // El puntero apunta al dato

  printf("Valor del puntero p: %p\n", p);
  printf("Resultado de p+1: %p\n", p+1);
}