#include "calculadora.h"
#include <stdio.h> 

int main(int argc, char *argv[]) {
    CLIENT *clnt; // contendrá información sobre el servicio remoto que buscamos
    Operandos op;   // Esta variable será el parámetro al servicio
    int *resultado; // Y aquí guardaremos la respuesta del servicio
    ResultadoDivision * resDivision = NULL;
    // Inicializar la estructura con la información del servicio buscado
    // Hay que pasarle:
    //  1. IP o nombre de la máquina donde está el servidor (la obtenemos
    //     en este caso de la línea de comandos)
    //  2. Número de programa (o la constante que lo representa)
    //  3. Número de versión (o la constante que lo representa)
    //  4. Protocolo "tcp" o "udp"
    clnt = clnt_create(argv[1], CALCULADORA, BASICA, "udp");
    if (clnt==NULL) {
        clnt_pcreateerror("No puedo inicializar cliente");
        exit(1);
    }

    // ... A completar: inicialización de la variable op
    //     pidiéndole al usuario un par de números a sumar
    op.a = 5;
    op.b = 5;

    // SUMA
    // Realizar invocacion remota 
    resultado = sumar_1(&op, clnt);
    // Mostrar resultado, recordar que devuelve un puntero
    if (resultado == NULL) {
        clnt_perror(clnt, "Fallo en la invocacion remota");
        exit(1);
    }
    printf("Respuesta: %d + %d = %d\n",op.a,op.b,*resultado);

    // RESTA
    // Realizar invocacion remota 
    resultado = restar_1(&op, clnt);
    // Mostrar resultado, recordar que devuelve un puntero
    if (resultado == NULL) {
        clnt_perror(clnt, "Fallo en la invocacion remota");
        exit(1);
    }
    printf("Respuesta: %d - %d = %d\n",op.a,op.b,*resultado);

    // MULTIPLICACION
    // Realizar invocacion remota 
    resultado = multiplicar_1(&op, clnt);
    // Mostrar resultado, recordar que devuelve un puntero
    if (resultado == NULL) {
        clnt_perror(clnt, "Fallo en la invocacion remota");
        exit(1);
    }
    printf("Respuesta: %d * %d = %d\n",op.a,op.b,*resultado);

    // DIVISION
    // Realizar invocacion remota 
    resDivision = dividir_1(&op, clnt);
    // Mostrar resultado, recordar que devuelve un puntero
    if (resDivision == NULL) {
        clnt_perror(clnt, "Fallo en la invocacion remota");
        exit(1);
    }
    switch (resDivision->tipo)
    {
    case 1:
        printf("Respuesta: %d / %d = %d\n",op.a,op.b, resDivision->ResultadoDivision_u.n);
        break;
    case 2:
        printf("Respuesta: %d / %d = %f\n",op.a,op.b, resDivision->ResultadoDivision_u.x);
        break;
    case 3:
        printf("Respuesta: %s\n", resDivision->ResultadoDivision_u.error);
        break;
    default:
        break;
    }

    clnt_destroy(clnt);
    return 0;
}