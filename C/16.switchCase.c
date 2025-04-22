#include "shit.h"

int main(void)
{
    int no;

    printf("Enter an integer: "); scanf("%d", &no);

    switch(no % 3)
    {
        case 0 : printf("%d is divisible by 3.\n", no); break;
        case 1 : printf("The remainder of %d divided by 3 is %d.\n", no, no % 3); break;
        case 2 : printf("The remainder of %d divided by 3 is %d.\n", no, no % 3); break;
    }

    return 0;
}