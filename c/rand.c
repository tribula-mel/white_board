#include <stdio.h>
#include <stdlib.h>

int main(void)
{
   int i;
   int j;
   unsigned int seed;

   for(i=0; i < 100; ++i) {
      j = rand_r(&seed);
      j = j & 0xF;
      if (j >= 9) j = j >> 1;
      printf("%d seed(%x)\n", j, seed);
   }
		    
   return EXIT_SUCCESS;
}
