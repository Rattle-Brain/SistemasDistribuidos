#include <stdio.h>
int main()
{
    int* p_i = NULL;
    *p_i = 100;

    printf("%p", p_i); 
}