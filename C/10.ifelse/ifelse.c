#include "../shit.h"

int main(void)
{
    int no;

    printf("Enter an integer:"); scanf("%d", &no);

    if (no % 5)
    {
        puts("The number cannot be devided by 5."); 
    }
    else 
    {
        puts("The number can be devided by 5.");
    }

    return 0;
}