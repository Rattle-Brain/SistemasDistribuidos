#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipos.h"
#include <rpc/xdr.h>

#define MAX_TEXTO 100

int main() {
    // Abrir el archivo Resultado.dat para lectura binaria
    FILE *archivo = fopen("Resultado.dat", "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo Resultado.dat");
        return 1;
    }
    
    // Declarar la variable de tipo Resultado para almacenar los datos decodificados
    struct Resultado resultado;
    
    // Crear un flujo XDR para leer desde el archivo
    XDR xdrs;
    xdrstdio_create(&xdrs, archivo, XDR_DECODE);
    
    // Leer los datos del archivo y almacenarlos en la variable de tipo Resultado utilizando el filtro xdr_Resultado()
    if (!xdr_Resultado(&xdrs, &resultado)) {
        fprintf(stderr, "Error al leer los datos del archivo Resultado.dat\n");
        return 1;
    }
    
    // Destruir el flujo XDR y cerrar el archivo
    xdr_destroy(&xdrs);
    fclose(archivo);
    
    // Mostrar los datos decodificados
    switch (resultado.caso) {
        case 1:
            printf("Entero: %d\n", resultado.Resultado_u.n);
            break;
        case 2:
            printf("Real: %f\n", resultado.Resultado_u.x);
            break;
        case 3:
            printf("Texto: %s\n", resultado.Resultado_u.error);
            break;
        default:
            fprintf(stderr, "Tipo de dato desconocido\n");
            return 1;
    }
    
    return 0;
}