#include "../shit.h"

int main(void)
{
	int retry;

	do
	{
		int i, no;

		do
		{
			printf("Enter a positive integer:");
			scanf("%d", &no);

			if (no <= 0)
				puts("Do not enter a non-positive integer!");
		} while (no <= 0);

		for (i = 1; i <= no; i++)
			putchar('*');
		putchar('\n');
		
		printf("Continue? [Yes(0)/No(9)]:");
		scanf("%d", &retry);
	} while (retry == 0);

	return 0;
}