#include "shit.h"

int main(void)
{
    int n1, n2, max;

    puts("Enter 2 integers.");
    printf("Integer 1:"); scanf("%d", &n1);
    printf("Integer 2:"); scanf("%d", &n2);

    max = (n1 > n2) ? n1 : n2;

    printf("The bigger one is: %d.\n", max);

    return 0;
}