#include <stdio.h>

#define MEL(test) test("hello");

#define MEL1(a)  a

typedef struct mut {
   int a;
   int b;
} mut_t;

static void test(char *string)
{
   printf("funcction test[%s]\n", string);
}

int main(void)
{
   mut_t m1 = {1,2};
   mut_t *m2 = &m1;

   MEL(test)
   MEL(printf)

   printf("\nm1(%d,%d)\n", m1.a, m1.b);
   printf("m2(%d,%d)\n", m2->MEL1(a), m2->MEL1(b));

   return 0;
} 
