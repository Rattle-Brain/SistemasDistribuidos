#include <stdio.h>
#include <stdlib.h>
#include "saludos.h"

void printl(char[]);

int main()
{
    int i;
    for(i = 0; i < NUMSALUDOS; i++)
    {
        printl("Hola");
    }
    exit(0);
}