#include "../shit.h"

int main(void)
{
	int i, no;

	printf("Enter a positive integer:");
	scanf("%d", &no);

	for (i = 0; i <= no; i++)
		printf("%d ", i);
	putchar('\n');

	return 0;
}