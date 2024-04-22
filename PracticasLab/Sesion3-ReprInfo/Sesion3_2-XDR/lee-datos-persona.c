#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipos.h"
#include <rpc/xdr.h>


int main() {
    // Abrir el archivo Persona.dat para lectura binaria
    FILE *archivo = fopen("Persona.dat", "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo Persona.dat");
        return 1;
    }
    
    // Declarar la variable de tipo Persona para almacenar los datos decodificados
    struct Persona persona;
    
    // Crear un flujo XDR para leer desde el archivo
    XDR xdrs;
    xdrstdio_create(&xdrs, archivo, XDR_DECODE);
    
    // Leer los datos del archivo y almacenarlos en la variable de tipo Persona utilizando el filtro xdr_Persona()
    if (!xdr_Persona(&xdrs, &persona)) {
        fprintf(stderr, "Error al leer los datos del archivo Persona.dat\n");
        return 1;
    }
    
    // Destruir el flujo XDR y cerrar el archivo
    xdr_destroy(&xdrs);
    fclose(archivo);
    
    // Mostrar los datos decodificados de la persona
    printf("Persona decodificada:\n");
    printf("  Nombre: %s\n", persona.nombre);
    printf("  Edad: %d\n", persona.edad);
    
    return 0;
}