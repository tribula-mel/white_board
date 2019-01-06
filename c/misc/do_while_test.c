#include <stdio.h>

static int b_t(void)
{
   return 1;
}

int main(void)
{
   do {
      printf("first printf\n");
      if (b_t()) {
         continue;
      }
      printf("second printf\n");
   } while(1);

   return 0;
}

