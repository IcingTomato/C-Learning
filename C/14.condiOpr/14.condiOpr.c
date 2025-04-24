#include "../shit.h"

int main(void)
{
    int n1, n2;

    puts("Enter 2 integers.");
    printf("Integer 1:"); scanf("%d", &n1);
    printf("Integer 2:"); scanf("%d", &n2);

    printf("Their difference is %d.\n", (n1 > n2) ? n1 - n2 : n2 - n1);

    return 0;
}