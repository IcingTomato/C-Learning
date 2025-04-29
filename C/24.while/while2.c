#include "../shit.h"

int main(void)
{
	int no;

	printf("Enter a positive integer:"); scanf("%d", &no);

	while (no >= 0)
		printf("%d ", no--);

	printf("\n");

	return 0;
}