#include <stdio.h>
#include <rpc/xdr.h>
#include <stdlib.h>
#include <string.h>
#include "tipos.h"

#define MAX_TEXTO 100

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
    
    // Abre el archivo Persona.dat para escritura binaria
    FILE *file_person = fopen("Persona.dat", "wb");
    if (file_person == NULL) {
        perror("Error al abrir el archivo Persona.dat");
        exit(EXIT_FAILURE);
    }
    
    // Crea un flujo XDR para escribir en el archivo
    XDR xdr_person;
    xdrstdio_create(&xdr_person, file_person, XDR_ENCODE);
    
    // Volca la variable de tipo Persona al archivo utilizando el filtro xdr_Persona()
    if (!xdr_Persona(&xdr_person, &persona)) {
        fprintf(stderr, "Error al volcar la variable de tipo Persona al archivo\n");
        exit(EXIT_FAILURE);
    }
    
    // Destruye el flujo XDR y cierra el archivo
    xdr_destroy(&xdr_person);
    fclose(file_person);
    
    printf("Se ha volcado la variable de tipo Persona al archivo Persona.dat correctamente.\n");
    
    // PARTE 3 - RESULTADO

    // Presentar el menú al usuario
    printf("Elige una opcion:\n");
    printf("    1. Entero\n");
    printf("    2. Real\n");
    printf("    3. Texto\n");
    printf("Tu eleccion: ");
    
    // Leer la elección del usuario
    int opcion;
    scanf("%d", &opcion);
    
    // Validar la elección del usuario
    if (opcion < 1 || opcion > 3) {
        fprintf(stderr, "Opcion invalida. Debe ser 1, 2 o 3.\n");
        exit(EXIT_FAILURE);
    }
    
    // Asignar la elección del usuario al discriminante de la variable de tipo Resultado
    resultado.caso = opcion;
    
    // Pedir al usuario el dato correspondiente según su elección
    switch (opcion) {
        case 1:
            printf("Introduce un numero entero: ");
            scanf("%d", &resultado.Resultado_u.n);
            break;
        case 2:
            printf("Introduce un numero real: ");
            scanf("%f", &resultado.Resultado_u.x);
            break;
        case 3:
            printf("Introduce un texto: ");
            // Hay que hacer un Mallo antes de nada o SIGSEGV
            resultado.Resultado_u.error = malloc(sizeof(char) * MAX_TEXTO);
            scanf("%s", resultado.Resultado_u.error);
            break;
    }

    perror("Variable asignada a campo Resultado\n");
    
    // Abre el archivo Resultado.dat para escritura binaria
    FILE *file_result = fopen("Resultado.dat", "wb");
    if (file_result == NULL) {
        perror("Error al abrir el archivo Resultado.dat");
        exit(EXIT_FAILURE);
    }
    perror("Resultado.dat creado\n");
    
    // Crea un flujo XDR para escribir en el archivo
    XDR xdr_result;
    xdrstdio_create(&xdr_result, file_result, XDR_ENCODE);

    perror("XDR creado\n");
    
    // Volca la variable de tipo Resultado al archivo utilizando el filtro xdr_Resultado()
    if (!xdr_Resultado(&xdr_result, &resultado)) {
        fprintf(stderr, "Error al volcar la variable de tipo Resultado al archivo\n");
        exit(EXIT_FAILURE);
    }

    perror("xdr_Resultado ejecutada correctamente\n");
    
    // Destruye el flujo XDR y cierra el archivo
    xdr_destroy(&xdr_result);
    fclose(file_result);
    
    printf("Se ha volcado la variable de tipo Resultado al archivo Resultado.dat correctamente.\n");

    exit(EXIT_SUCCESS);
}