#include "../shit.h"

int main(void)
{
	int no;

	printf("Please enter a positive integer:"); scanf("%d", &no);

	while (no-- > 0)
		putchar('*');
	putchar('\n');

	return 0;
}