#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipos.h"
#include <rpc/xdr.h>

int main()
{
    // Abrir el archivo Texto.dat para lectura binaria
    FILE *file_text = fopen("Texto.dat", "rb");
    if (file_text == NULL) {
        perror("Error al abrir el archivo Texto.dat");
        return 1;
    }
    
    // Declarar la variable de tipo Texto para almacenar los datos decodificados
    Texto texto;
    
    // Crear un flujo XDR para leer desde el archivo
    XDR xdr_text;
    xdrstdio_create(&xdr_text, file_text, XDR_DECODE);
    
    // Leer los datos del archivo y almacenarlos en la variable de tipo Texto utilizando el filtro xdr_Texto()
    if (!xdr_Texto(&xdr_text, &texto)) {
        fprintf(stderr, "Error al leer los datos del archivo Texto.dat\n");
        return 1;
    }
    
    // Destruir el flujo XDR y cerrar el archivo
    xdr_destroy(&xdr_text);
    fclose(file_text);
    
    // Mostrar el texto decodificado
    printf("Texto decodificado: %s\n", texto);
    
    return 0;
}