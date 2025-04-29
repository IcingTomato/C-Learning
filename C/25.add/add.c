#include "../shit.h"

int main(void)
{
	int i, no;

	printf("Enter a positive integer:"); scanf("%d", &no);

	i = 0;

	while (i <= no)
		printf("%d ", i++);
	printf("\n");

	return 0;
}