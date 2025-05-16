#include "../shit.h"

int main(void)
{
	int i;
	int v[5];

	for (i = 0; i < 5; i++)
		v[i] = i + 1;

	// 遍历 traverse

	for (i = 0; i < 5; i++)
		printf("v[%d] = %d\n", i, v[i]);

	return 0;
}