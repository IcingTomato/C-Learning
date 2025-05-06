#include "../shit.h"

int main(void)
{
	int i, j, len;

	puts("Generates an isosceles right-angled triangle with a right angle at the top left.");

	printf("Short side:"); scanf("%d", &len);

	for (i = 1; i <= len; i++)
	{
		for (j = 1; j <= len - i; j++)
			putchar('*');
		putchar('\n');
	}

	return 0;
}