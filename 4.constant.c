# include <stdio.h>

int main(void)
{
    int a, b;
    
    puts("Enter two integers: ");
    scanf("%d %d", &a, &b);

    printf("The sum is %d\n", a + b);
    printf("The difference is %d\n", a - b);
    printf("The product is %d\n", a * b);
    printf("The quotient is %d\n", a / b);
    printf("The remainder is %d\n", a % b);
    printf("The average is %d\n", (a + b) / 2);
    printf("The average in floating-point is %.2f\n", (float)(a + b) / 2);
    printf("The sum of squares is %d\n", a * a + b * b);
    printf("The sum of cubes is %d\n", a * a * a + b * b * b);

    return 0;
} 
