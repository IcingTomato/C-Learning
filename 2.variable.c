#include <stdio.h>

int  main (void)
{
    int vx, vy;
    printf("X is: %d\n", vx);
    printf("Y is: %d\n", vy);

    int x, y;
    printf("Enter X: ");
    scanf("%d", &x);
    printf("Enter Y: ");
    scanf("%d", &y);
    printf("3*X is: %d\n", x * 3);
    printf("Y/2 is: %d\n", y / 2);
    return 0;
}