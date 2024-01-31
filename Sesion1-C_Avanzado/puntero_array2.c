#include <stdio.h>

int main()
{
    int a[] = {1,2,3,4,5};
    int *p;
    int i;

    p = a;
    for(i=0; i<5; i++)
        printf("a[%d] = %d\n", i, *(p+i));
    return 0;
}