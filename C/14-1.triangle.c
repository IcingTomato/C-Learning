#include "shit.h"

int main(void)
{
    int a, b, c;

    // 输入三边
    puts("Enter three sides of a triangle (a, b, c):");
    scanf("%d %d %d", &a, &b, &c);

    // 判断是否为三角形
    if (a > 0 && b > 0 && c > 0 && (a + b > c) && (a + c > b) && (b + c > a)) 
    {
        printf("The numbers %d, %d, and %d can form a triangle.\n", a, b, c);
    } 
    else 
    {
        printf("The numbers %d, %d, and %d cannot form a triangle.\n", a, b, c);
    }

    return 0;
}