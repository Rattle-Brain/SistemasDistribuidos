#include <stdio.h>
#include <stdlib.h>

int main() {
    int *p_int = 0;  // Puntero a entero, inicializado como nulo

    p_int = malloc(sizeof(int));  // Solicitamos memoria
    if (p_int == NULL) {  // Verificar si se ha obtenido
       printf("No hay memoria suficiente");
       return(-1);
    }

    // Si lo tenemos, lo podemos usar
    *p_int = 5;
    *p_int += 3;
    *p_int = *p_int * *p_int;
    printf("Resultado: %d\n", *p_int);

    // Hemos terminado. Liberar la memoria antes asignada
    free(p_int);
}