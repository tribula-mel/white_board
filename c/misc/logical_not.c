#include <stdio.h>

int main(void) {
   char a, b, c;

   a = 0;
   b = 1;
   c = 20;

   printf("a(%d) !a(%d)\n", a, !a);
   printf("b(%d) !b(%d)\n", b, !b);
   printf("c(%d) !c(%d)\n", c, !c);

   return 0;
}
