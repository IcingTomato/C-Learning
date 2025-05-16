#include "../shit.h"

int main(void)
{
	int i;
	// int v[5] = {1, 2, 3, 4, 5, };
	int v[] = {1, 2, 3, 4, 5, };
	int b[10];


	for (i = 0; i < 5; i++)
		b[i] = v[i];

	// 遍历 traverse

	puts("v   b");
	puts("-----");
	for (i = 0; i < 5; i++)
		printf("%d   %d\n", v[i], b[i]);

	return 0;
}