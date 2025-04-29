#include "../shit.h"

int main(void)
{
	int i = 0;
	int sum = 0;
	int num, tmp;

	printf("How many integers do you want to enter?"); scanf("%d", &num);

	while (i < num)
	{
		printf("No.%d:", ++i); scanf("%d", &tmp);
		sum += tmp;
	}

	printf("Sum is %d, average is %.2f.\n", sum, (double)sum / num);

    printf("Press any key to continue.\n"); getchar();
    
    return 0;
}