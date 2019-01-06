#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

static int pcx(int fd, int size, char *name);

static int pcx(int fd, int size, char *name)
{
   int out_fd = -1;
   int i = 0;
   int rc = -1;

   unsigned char byte = 0;

   out_fd = open(name, O_WRONLY | O_CREAT);
   if (out_fd == -1) {
      printf("line[%d] open error: %s\n", __LINE__, strerror(errno));
      return -1;
   }

   for (i = 0; i < size; i++) {
      rc = read(fd, &byte, 1);
      if (rc == 0) {
         printf("reached end of file???\n");
         goto error;
      }
      if (rc == -1) {
         printf("line[%d] read error: %s\n", __LINE__, strerror(errno));
         goto error;
      }

      write(out_fd, &byte, 1);
   }

   close(out_fd);

   return 0;

error:
   close(out_fd);

   return -1;
}

int main(int argc, char **argv)
{
   int rc = -1;
   int fd = -1;
   unsigned int size = 0;

   unsigned char sync = 0;
   char name[13] = {0};
   char blob[66] = {0};

   struct stat buf;

   if (argc != 2) {
      printf("usage: %s <file_name>\n", argv[0]);
      return EXIT_FAILURE;
   }

   rc = stat(argv[1], &buf);
   if (rc == -1) {
      printf("stat error: %s\n", strerror(errno));
      return EXIT_FAILURE;
   }
   fd = open(argv[1], O_RDONLY);
   if (fd == -1) {
      printf("open error: %s\n", strerror(errno));
      return EXIT_FAILURE;
   }

   /* library header, not interesting at all */
   lseek(fd, 122, SEEK_SET);
   while(1) {
      rc = read(fd, &sync, 1);
      if (rc == 0) {
         printf("end of file reached\n");
         return EXIT_FAILURE;
      }
      if (rc == -1) {
         printf("read error: %s\n", strerror(errno));
         return EXIT_FAILURE;
      }
      if (sync != 1) {
         break;
      }
      rc = read(fd, &name[0], sizeof(name));
      if (rc == 0) {
         printf("end of file reached\n");
         return EXIT_FAILURE;
      }
      if (rc == -1) {
         printf("read error: %s\n", strerror(errno));
         return EXIT_FAILURE;
      }
      rc = read(fd, &size, sizeof(size));
      if (rc == 0) {
         printf("end of file reached\n");
         return EXIT_FAILURE;
      }
      if (rc == -1) {
         printf("read error: %s\n", strerror(errno));
         return EXIT_FAILURE;
      }
      rc = read(fd, &blob[0], sizeof(blob));
      if (rc == 0) {
         printf("end of file reached\n");
         return EXIT_FAILURE;
      }
      if (rc == -1) {
         printf("read error: %s\n", strerror(errno));
         return EXIT_FAILURE;
      }
      rc = pcx(fd, size, name);
      if (rc == -1) {
         printf("convertion error\n");
         return EXIT_FAILURE;
      }
   }

   close(fd);

   return EXIT_SUCCESS;
}
