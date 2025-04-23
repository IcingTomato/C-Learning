#include "shit.h"

int main(void)
{
    char retry[4];

    do
    {
        int no;

        printf("Enter an integer: "); scanf("%d", &no);

        if (no % 2)
            puts("Odd.");
        else
            puts("Even.");
        printf("Retry? [yes/no] :"); scanf("%3s", retry);
    }
    while (strcmp(retry, "yes") == 0);

    return 0;
}