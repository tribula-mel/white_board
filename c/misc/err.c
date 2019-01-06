#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
   int fd = - 1;
   int rc = - 1;
   char c = 0;

   errno = ENOENT;

   printf("forcing ENOENT on errno(%s)\n", strerror(errno));

   fd = open("/home/bpetrovi/mel", O_RDONLY, 0);
   if (fd < 0) {
      printf("open() failed (%s)\n", strerror(errno));
   }
   rc = read(fd, &c, 1);
   if (rc < 0) {
      printf("read() failed (%s)\n", strerror(errno));
      close(fd);
   }
   printf("c(%x)\n", c);
   close(fd);

   return 0;
}
