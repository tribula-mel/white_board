#include <stdio.h>

int main(void)
{
	int ***tp;
	int **dp;
	int *p;
	int i = 13;

	p = &i;
	dp = &p;
	tp = &dp;

	printf("triple pointer[%p], double pointer[%p], pointer[%p]\n",\
		tp, dp, p);
	printf("deref triple pointer[%p]\n", *tp);
	printf("deref double pointer[%p]\n", *dp);
	printf("deref pointer[%x]\n", *p);

	return 0;
}
