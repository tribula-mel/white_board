#include <string.h>
#include <stdio.h>

int main(void) {
   char bug[] = "test";
   char *status = strchr(bug, '=') + 2;

   printf("status[%p]\n", (void *)status);

   return 0;
}
