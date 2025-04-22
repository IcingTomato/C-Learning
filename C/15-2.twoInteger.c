#include "shit.h"

int main(void)
{
    int a, b;

    puts("Enter 2 integers:");
    scanf("%d %d", &a, &b);

    int diff = (a > b) ? a - b : b - a;

    if (diff <= 10)
    {
        puts("diff <= 10");
    }
    else
    {
        puts("diff >= 11");
    }

    return 0;
}