#include <stdlib.h>

int main(void)
{
   char *pp = NULL;

   free(pp);

   pp = malloc(10);

   free(pp);
   free(pp);

   return 0;
}
