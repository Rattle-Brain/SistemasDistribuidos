#include <stdio.h>
#include <stdlib.h>
#include "tipos.h"
#include <rpc/xdr.h>

void imprimir_lista(Lista *ptr) {
    // Iterar sobre la lista e imprimir el valor del campo dato
    while (ptr != NULL) {
        printf("%d ", ptr->dato);
        ptr = ptr->siguiente;
    }
    printf("\n");
}

int main() {
    // Declarar una variable de tipo Lista para almacenar los datos decodificados
    Lista lista;
    lista.siguiente = NULL;

    // Abrir el archivo Lista.dat para lectura binaria
    FILE *archivo = fopen("Lista.dat", "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo Lista.dat");
        return 1;
    }

    // Crear un flujo XDR para leer desde el archivo
    XDR xdrs;
    xdrstdio_create(&xdrs, archivo, XDR_DECODE);

    // Leer los datos del archivo y almacenarlos en la variable de tipo Lista utilizando el filtro xdr_Lista()
    if (!xdr_Lista(&xdrs, &lista)) {
        fprintf(stderr, "Error al leer los datos del archivo Lista.dat\n");
        return 1;
    }

    // Destruir el flujo XDR y cerrar el archivo
    xdr_destroy(&xdrs);
    fclose(archivo);

    // Imprimir el valor del primer elemento de la lista
    printf("Valor del primer elemento de la lista: %d\n", lista.dato);

    // Imprimir el resto de los elementos de la lista
    printf("Resto de elementos de la lista: \n");
    imprimir_lista(lista.siguiente);

    return 0;
}