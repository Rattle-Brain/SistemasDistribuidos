#include <stdio.h>
#include <stdlib.h>
#include "saludos.h"

int main(int argc, char** argv)
{
    printf("Numero de parametros: %d\n", argc-1);
    if(argv > 0)
    {
        int i;
        for(i = 1; i < argc; i++)
        {
            printl(argv[i]);
        }
    }

}