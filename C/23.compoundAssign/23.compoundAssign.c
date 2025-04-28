#include "../shit.h"

int main(void)
{
    int sum = 0;
    int cnt = 0;
    int retry;

    do
    {
        int t;
        printf("Enter an integer:"); scanf("%d", &t);

        sum += t;
        cnt++;

        printf("Continue?  [Yes...0/No...9]:"); scanf("%d", &retry);
    } while (retry == 0);

    printf("Sum is %d, average is %.2f.\n", sum, (double)sum / cnt);
    
    return 0;
}