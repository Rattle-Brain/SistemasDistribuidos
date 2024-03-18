#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *p = NULL;  // Puntero al array, aún no creado
    int tamanio;    // Tamaño deseado para el array
    int i;          // para bucles

    printf("Numero de elementos deseado: ");
    scanf("%d", &tamanio);
    // Ahora creamos espacio para albergar ese numero de enteros
    p = malloc(tamanio * sizeof(int)); // malloc espera numero de bytes
    if (p==NULL) {
        printf("No hay memoria suficiente\n");
        exit(1);
    }

    // Y ahora recorremos el array. Observa que:
    //   p apunta al primer elemento
    //   *p es el valor del primer elemento
    //   *p es lo mismo que *(p+0)
    //   y por tanto lo mismo que p[0]
    //   Análogamente *(p+i) es el valor del elemento iesimo
    //   y lo mismo que p[i]
    for (i=0; i<tamanio; i++)
        p[i] = i*2;

    // Mostramos los valores introducidos
    for (i=0; i<tamanio; i++)
        printf("p[%d] = %d\n", i, p[i]);

    // Liberamos el espacio que ya no necesitamos
    free(p);
    return 0;
}