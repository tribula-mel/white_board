#include <stdio.h>

int main(void)
{
	int a = 51999;
	float b = 0;

	b = (float) a / 1000;
	printf("b=(%3.3f)\n", b);

	return 0;

}
