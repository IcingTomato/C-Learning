#include "../shit.h"

int main(void)
{
	int i;
	double x[7];

	for (i = 0; i < 7; i++)
		x[i] = i + 1;

	// 遍历 traverse

	for (i = 0; i < 7; i++)
		printf("x[%d] = %.1f\n", i, x[i]);

	return 0;
}