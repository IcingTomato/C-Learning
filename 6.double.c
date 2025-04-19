#include "shit.h"

int main(void)
{
    double x, y;

    puts("Enter 2 numbers:");
    printf("Real x:"); scanf("%lf", &x);
    printf("Real y:"); scanf("%lf", &y);

    printf("x + y = %f\n", x + y);
    printf("x - y = %f\n", x - y);
    printf("x * y = %f\n", x * y);
    printf("x / y = %f\n", x / y);

    return 0;
}