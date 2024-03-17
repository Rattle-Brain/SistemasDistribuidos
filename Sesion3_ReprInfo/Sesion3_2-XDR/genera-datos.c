#include <stdio.h>
#include <rpc/xdr.h>
#include <stdlib.h>
#include "tipos.h"

int main() {

    // PARTE 0 - Declara variables
    // Declara una variable de tipo Texto
    Texto texto;
    
    // Declara una variable de tipo Persona
    struct Persona persona;
    
    // Declara una variable de tipo Resultado
    struct Resultado resultado;
    
    // Imprime un mensaje para indicar que se han declarado las variables
    printf("Se han declarado las variables correctamente.\n");
    
    // PARTE 1 - TEXTO

    // Asigna la cadena "Probando" a la variable de tipo Texto
    texto = "Probando";
    
    // Abre el archivo Texto.dat para escritura binaria
    FILE *file_text = fopen("Texto.dat", "wb");
    if (file_text == NULL) {
        perror("Error al abrir el archivo Texto.dat\n");
        exit(EXIT_FAILURE);
    }
    
    // Crea un flujo XDR para escribir en el archivo
    XDR xdr_text;
    xdrstdio_create(&xdr_text, file_text, XDR_ENCODE);
    
    // Volca la variable de tipo Texto al archivo usando el filtro xdr_Texto()
    if (!xdr_Texto(&xdr_text, &texto)) {
        perror("Error al volcar la variable de tipo Texto al archivo\n");
        exit(EXIT_FAILURE);
    }
    
    // Destruye el flujo XDR y cierra el archivo
    xdr_destroy(&xdr_text);
    fclose(file_text);
    
    printf("Se ha volcado la variable de tipo Texto al archivo Texto.dat correctamente.\n");
    
    // PARTE 2 - PERSONA

    persona.nombre = "Maria";
    persona.edad = 22;

    perror("Variables asignadas correctamente -Persona\n");
    
    // Abre el archivo Persona.dat para escritura binaria
    FILE *file_person = fopen("Persona.dat", "wb");
    if (file_person == NULL) {
        perror("Error al abrir el archivo Persona.dat");
        exit(EXIT_FAILURE);
    }

    perror("Fichero abierto correctamente\n");
    
    // Crea un flujo XDR para escribir en el archivo
    XDR xdr_person;
    xdrstdio_create(&xdr_person, file_person, XDR_ENCODE);
    perror("Filtro XDR_person creado correctamente\n");
    
    // Volca la variable de tipo Persona al archivo utilizando el filtro xdr_Persona()
    if (!xdr_Persona(&xdr_person, &persona)) {
        fprintf(stderr, "Error al volcar la variable de tipo Persona al archivo\n");
        exit(EXIT_FAILURE);
    }

    perror("Llamada a xdr_Persona() terminada correctamente\n");
    
    // Destruye el flujo XDR y cierra el archivo
    xdr_destroy(&xdr_person);
    fclose(file_person);
    
    printf("Se ha volcado la variable de tipo Persona al archivo Persona.dat correctamente.\n");
    
    exit(EXIT_SUCCESS);
}