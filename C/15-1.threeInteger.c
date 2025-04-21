#include "shit.h"

int main(void)
{
    int a, b, c;

    puts("Enter 3 numbers:");
    scanf("%d %d %d", &a, &b, &c);

    if (a == b && a == c)
    {
        puts("Three equal.");
    }
    else if (a == b || a == c || b == c)
    {
        puts("Two of them equal.");
    }
    else
    {
        puts("None of them equal.");
    }

    return 0;
}