#include <stdio.h>
#include <stdlib.h>

int main()
{
    char *txt1;
    char *txt2;

    txt1 = "Hola";
    txt2 = "mundo";
    printf("%s, %s\n", txt1, txt2);
    printf("txt1 apunta a %p\n", txt1);
    printf("txt2 apunta a %p\n", txt2);
    return 0;
}