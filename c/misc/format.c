#include <stdio.h>

int main(void)
{
	float   a = 3.1415;
	double a1 = 3.14151617;
	int     b = 2;

	printf("%02d%09.6f,%c,\n", b, a, 'N');
	printf("%02d%09.6lf,%c,\n", b, a1, 'N');
	printf("%011.8lf,\n", a1);
	printf("%02.0lf,\n", a1);

	return 0;
}
