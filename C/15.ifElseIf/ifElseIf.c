#include "../shit.h"

int main(void)
{
	int month;

	printf("Enter month: ");
	scanf("%d", &month);

	if (month >= 3 && month <= 5)
		printf("%d is spring.\n", month);
	else if (month >= 6 && month <= 8)
		printf("%d is summer.\n", month);
	else if (month >= 9 && month <= 11)
		printf("%d is autumn.\n", month);
	else if (month == 1 || month == 2 || month == 12)
		printf("%d is winter.\n", month);
	else
		printf("%d is null.\a\n", month);

	return 0;
}
