#include <stdio.h>
#include <rpc/xdr.h>
#include "tipos.h"

int main() {
    // Declara una variable de tipo Texto
    Texto texto;
    
    // Declara una variable de tipo Persona
    struct Persona persona;
    
    // Declara una variable de tipo Resultado
    struct Resultado resultado;
    
    // Imprime un mensaje para indicar que se han declarado las variables
    printf("Se han declarado las variables correctamente.\n");
    
    // Asigna la cadena "Probando" a la variable de tipo Texto
    texto = "Probando";
    
    // Abre el archivo Texto.dat para escritura binaria
    FILE *archivo = fopen("Texto.dat", "wb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo Texto.dat\n");
        return 1;
    }
    
    // Crea un flujo XDR para escribir en el archivo
    XDR xdrs;
    xdrstdio_create(&xdrs, archivo, XDR_ENCODE);
    
    // Volca la variable de tipo Texto al archivo usando el filtro xdr_Texto()
    if (!xdr_Texto(&xdrs, &texto)) {
        perror("Error al volcar la variable de tipo Texto al archivo\n");
        return 1;
    }
    
    // Destruye el flujo XDR y cierra el archivo
    xdr_destroy(&xdrs);
    fclose(archivo);
    
    printf("Se ha volcado la variable de tipo Texto al archivo Texto.dat correctamente.\n");
    
    return 0;
}