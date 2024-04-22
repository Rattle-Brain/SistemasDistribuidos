#include <stdio.h>
#include <stdlib.h>
#include <rpc/xdr.h>

int main() {
    // Declarar variables
    int numero;
    char caracter;

    // Pedir al usuario el valor para la variable int
    printf("Introduce un número entero: ");
    scanf("%d", &numero);

    // Pedir al usuario el valor para la variable char
    printf("Introduce un carácter: ");
    scanf(" %c", &caracter);  // El espacio antes de %c es importante para consumir el salto de línea anterior

    // Abrir el archivo para escritura en modo binario
    FILE *archivo = fopen("basicos.dat", "wb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    // Crear un flujo XDR para escribir en el archivo
    XDR xdrs;
    xdrstdio_create(&xdrs, archivo, XDR_ENCODE);

    // Escribir el valor del entero
    if (!xdr_int(&xdrs, &numero)) {
        perror("Error al escribir el entero en el archivo\n");
        exit(EXIT_FAILURE);
    }

    // Escribir el valor del caracter
    if (!xdr_char(&xdrs, &caracter)) {
       perror("Error al escribir el carácter en el archivo\n");
        exit(EXIT_FAILURE);
    }

    // Cerrar el flujo XDR y el archivo
    xdr_destroy(&xdrs);
    fclose(archivo);
    printf("Datos escritos correctamente en el archivo 'basicos.dat'\n");

    return 0;
}