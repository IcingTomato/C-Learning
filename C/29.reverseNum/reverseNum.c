#include "../shit.h"

int main(void)
{
	int no;

	do
	{
		printf("Enter a positive integer:"); scanf("%d", &no);

		if (no <= 0)
			puts("\aDo not enter a negative integer or 0! Fuck you.");
	} while (no <= 0);

	printf("The reverse of num is ");
	while (no > 0)
	{
		printf("%d", no % 10);
		no /= 10;
	}
	puts(".");

	return 0;
}