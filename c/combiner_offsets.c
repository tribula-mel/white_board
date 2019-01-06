#include <stdio.h>

#define COMBINER_MAX_IRQ	54
#define COMBINER_IRQ(x)		(x) * 8 + 128

int main(void)
{
	int x = 0;
	int offset = 0;
	int ppi = 0;

	for (x = 0; x < COMBINER_MAX_IRQ; x++) {
		offset = COMBINER_IRQ(x);
		ppi = offset & ~31;
		printf("x[%d]: offset[%d] ppi[%d]\n", x, offset, ppi);
	}

	return 0;
}
