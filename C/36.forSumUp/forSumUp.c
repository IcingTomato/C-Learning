#include "../shit.h"

int main(void)
{
	int i = 0;
	int sum = 0;
	int num, tmp;

	printf("How many integers do you want to enter?");
	scanf("%d", &num);

	for (i = 0; i < num; i++)
	{
		printf("No.%d:", i + 1);
		scanf("%d", &tmp);
		sum += tmp;
	}

	printf("Sum is %d, average is %.2f.\n", sum, (double)sum / num);

    return 0;
}