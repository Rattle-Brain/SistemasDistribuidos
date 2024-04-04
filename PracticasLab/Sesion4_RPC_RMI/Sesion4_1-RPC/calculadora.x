/* Fichero calculadora.x */

/* Declaración de tipos xdr usados por las funciones */
/* (a rellenar por el alumno) */

/* A continuación va la descripción del interfaz en idl */
program CALCULADORA {
    version BASICA {
        int sumar(Operandos) = 1;
        int restar(Operandos) = 2;
        int multiplicar(Operandos) = 3;
        ResultadoDivision dividir(Operandos) = 4;
        /* Resto de las funciones del interfaz a rellenar por el alumno*/

    } = 1;  /* Este es el número de versión */
} = 0x40001234;  /* Este es el número de programa */

/* Operandos de entrada de la calculadora */
struct Operandos{
    int a;
    int b;
}; typedef struct Operandos Operandos;

/* Datos para retornar el resultado de la division */
typedef string Error<100>;
union ResultadoDivision switch(int tipo) {
    case 1: int n;
    case 2: float x;
    case 3: Error error;
}; typedef struct ResultadoDivision ResultadoDivision;



