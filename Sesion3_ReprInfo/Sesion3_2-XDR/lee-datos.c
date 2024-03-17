#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipos.h"
#include <rpc/xdr.h>

#define MAX_TEXTO 100

int main() {
    // Abrir el archivo Texto.dat para lectura binaria
    FILE *archivo = fopen("Texto.dat", "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo Texto.dat");
        return 1;
    }
    
    // Declarar la variable de tipo Texto para almacenar los datos decodificados
    Texto texto;
    
    // Crear un flujo XDR para leer desde el archivo
    XDR xdrs;
    xdrstdio_create(&xdrs, archivo, XDR_DECODE);
    
    // Leer los datos del archivo y almacenarlos en la variable de tipo Texto utilizando el filtro xdr_Texto()
    if (!xdr_Texto(&xdrs, &texto)) {
        fprintf(stderr, "Error al leer los datos del archivo Texto.dat\n");
        return 1;
    }
    
    // Destruir el flujo XDR y cerrar el archivo
    xdr_destroy(&xdrs);
    fclose(archivo);
    
    // Mostrar el texto decodificado
    printf("Texto decodificado: %s\n", texto);
    
    return 0;
}