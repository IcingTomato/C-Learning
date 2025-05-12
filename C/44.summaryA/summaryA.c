#include "../shit.h"

int main(void)
{
	int i, j;
	int x, y, z;

	do
	{
        printf("Enter a 0-100 integer: ");
		scanf("%d", &x);
    } while (x < 0 || x > 100);

	y = x;
	z = x;

	while (y >= 0)
		printf("%d %d\n", y--, ++z);

	printf ("The integer area %d of width and height"
			"rectangle's side is: \n", x);

	for (i = 1; i < x; i++)
	{
		if (i * i > x) 	break;
		if (x % i != 0) continue;
		printf("%d * %d\n", i, x / i);
	}

	puts("Five rows and seven columns of asterisks:");

	for (i = 1; i <= 5; i++)
	{
		for (j = 1; j <= 7; j++)
			putchar('*');
		putchar('\n');
	}

	return 0;
}