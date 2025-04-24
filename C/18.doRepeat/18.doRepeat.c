#include "../shit.h"

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
        printf("Retry? [y(es)/n(o)] :"); scanf("%3s", retry);
    }
    while (strcmp(retry, "no") != 0 && strcmp(retry, "n") != 0);

    return 0;
}