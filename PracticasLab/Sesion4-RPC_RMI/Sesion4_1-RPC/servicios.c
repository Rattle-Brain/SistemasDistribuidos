// Codigo de servicios.c
#include "calculadora.h"

int * sumar_1_svc(Operandos * op,  struct svc_req *r) // No usaremos r
{
    static int resultado; // Variable que contendrá la respuesta del servicio

    int a, b;

    a = op->a; //(*op).a
    b = op->b; //(*op).b

    resultado = a + b;
    return(&resultado);  // se retorna por referencia
}

int * restar_1_svc(Operandos * op,  struct svc_req *r) // No usaremos r
{
    static int resultado; // Variable que contendrá la respuesta del servicio

    int a, b;

    a = op->a; //(*op).a
    b = op->b; //(*op).b

    resultado = a - b;
    return(&resultado);  // se retorna por referencia
}

int * multiplicar_1_svc(Operandos * op,  struct svc_req *r) // No usaremos r
{
    static int resultado; // Variable que contendrá la respuesta del servicio

    int a, b;

    a = op->a; //(*op).a
    b = op->b; //(*op).b

    resultado = a * b;
    return(&resultado);  // se retorna por referencia
}

ResultadoDivision * dividir_1_svc(Operandos * op,  struct svc_req *r) // No usaremos r
{
    static ResultadoDivision resdiv; // Variable que contendrá la respuesta del servicio
    
    int a, b;

    a = op->a; //(*op).a
    b = op->b; //(*op).b


    if(b==0)
    {
        resdiv.tipo = 3;
        resdiv.ResultadoDivision_u.error = "No es posible dividir entre 0";
        return(&resdiv);  // se retorna por referencia
    }
    else if(a%b!=0)
    {
        resdiv.tipo = 2;
        resdiv.ResultadoDivision_u.x = ((float) a) / ((float) b);
        return(&resdiv);  // se retorna por referencia
    }
    else 
    {
        resdiv.tipo = 1;
        resdiv.ResultadoDivision_u.n = a/b;
        return(&resdiv);  // se retorna por referencia
    }

}
