#include <stdio.h>

int main(void)
{
   char a = 0xfe;
   unsigned char a1 = 0xfe;
   unsigned int b = 4;
   unsigned int c = 0;

   c = a + b;
   printf("c[%d] = a[%d] + b[%d]\n", c, a, b);

   c = 0;
   c = a1 +b;
   printf("c[%d] = a1[%d] + b[%d]\n", c, a1, b);

   return 0;
}
