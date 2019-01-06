#include <stdio.h>

#define SUFFIX " mel"

int main (void) {
	char comps[] = "boban petrovic" SUFFIX;

	printf("the composite string[%s]\n", comps);

	return 0;
}
