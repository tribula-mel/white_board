#include <stdio.h>

int main(void)
{
   int i = -1;

   if (i > -1) {
      printf("i > -1 test positive\n");
   }

   if (-1)
      printf("-1 test positive\n");

   if (10)
      printf("10 test positive\n");

   if (!-1)
      printf("!-1 test negative\n");

   if (!10)
      printf("!10 test negative\n");

   return 0;
}
