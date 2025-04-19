#include "shit.h"

int main(void) 
{
    int a = 10, b;

    printf("Enter: a is %d, b is %d\n", a, b);

	__asm__("movl %1, %%eax\n\t"
		"movl %%eax, %0\n\t"
		:"=r"(b)        /* output */
		:"r"(a)         /* input */
		:"%eax"         /* clobbered register */
		);

	printf("Result: a is %d, b is %d\n", a, b);

	return 0;
}