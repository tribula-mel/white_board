#include <stdio.h>

typedef unsigned int uint32;

static uint32 r_v(void) {
   return -1;
}

int main(void) {
   uint32 x = r_v();

   if (x == -1) {
      printf("x[%d]\n", x);
   }

   return 0;
}
