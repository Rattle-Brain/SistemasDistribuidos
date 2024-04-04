#include <stdio.h>


void ejemplo()
{
    static int contador = 0; // NO ES ASIGNACION, ES INICIALIZACION

    contador++;
    printf("Ejemplo he sido llamado %d veces\n", contador);
}

int main(){
    int i;

    for (i=0; i<4; i++) {
        ejemplo();
    }

    return 0;

}