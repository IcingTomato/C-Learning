#include "../shit.h"

int main(void)
{
    int a, b, x, y;

    printf("Enter A and B:");
    scanf("%d %d", &a, &b);

    x = a; y = b;

    // Right method
    int temp = a;
    a = b; b = temp;
    printf("After exchange, A and B: %d %d.\n", a, b);

    // Wrong method
    x = y; y = x;
    printf("If you use wrong method, A and B: %d %d.\n", x, y);

    return 0;
}