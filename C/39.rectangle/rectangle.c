#include "../shit.h"

int main(void)
{
	int i, j;
	int height, width;

	puts("Draw a rectangle.");
	printf("Height:"); 	scanf("%d", &height);
	printf("Width:");	scanf("%d", &width);

	for (i = 1; i <= height; i++)
	{
		for (j = 1; j <= width; j++)
			putchar('*');
		putchar('\n');
	}

	return 0;
}