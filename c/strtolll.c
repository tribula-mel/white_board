#include <stdlib.h>
#include <stdio.h>

int main(void) {
   char mel[]="00010";
   unsigned long long res = 0;

   res = strtoull(&mel[0], NULL, 10);

   printf("converted(%lld)\n", res);

   return 0;
}
