#include "../shit.h"

int main(void)
{
    int n1, n2;

    puts("Enter 2 integers:");
    printf("Integer 1:"); scanf("%d", &n1);
    printf("Integer 2:"); scanf("%d", &n2);

    if (n1 == n2)
        puts("Equal.");
    else
        puts("Not equal.");

    return 0;
}