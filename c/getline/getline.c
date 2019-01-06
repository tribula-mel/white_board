#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define LB_SIZE 2048

static char *extract_level(char *line) {
   int i = 0;
   char *p = line;

   for(i = 0; i < 4; i++) {
      if (*p == '\n' || *p == 0x0d) {
         /* end of line reached */
         break;
      }
      while(*p == ' ') {
         p++;
      }
      while(*p != ' ') {
         if (*p == '\n' || *p == 0x0d) {
            break;
         }
         p++;
      }
   }

   while(*p == ' ') {
      p++;
   }

   if (i != 4)
      return NULL;

   if (i == 4 && (*p == '\n' || *p == 0x0d))
      return NULL;

   return p;
}

int main(int argc, char **argv)
{
   FILE *fp = NULL;
   char *line_buffer = NULL;
   size_t lb_size = 0;
   int rc = 1;
   char *tok = NULL;
   int line_nc = 0;
   int line_d = 0;
   int line_v = 0;
   int line_i = 0;
   int line_e = 0;
   int line_w = 0;
   int line_unk = 0;
   int line_count = 0;

   if (argc != 2) {
      fprintf(stderr, "invalid number or arguments\n");
      fprintf(stderr, "invoke: %s file.txt\n", argv[0]);
      return EXIT_FAILURE;
   }

   fp = fopen(argv[1], "r");
   if (fp == NULL) {
      fprintf(stderr, "error opening %s: %s\n", argv[1], strerror(errno));
      return EXIT_FAILURE;
   }

   line_buffer = malloc(LB_SIZE);
   if (line_buffer == NULL) {
      fprintf(stderr, "malloc failed: %s\n", strerror(errno));
      goto exit_close;
   }
   lb_size = LB_SIZE;

   while(1) {
      rc = getline(&line_buffer, &lb_size, fp);
      if (rc == -1) {
         break;
      }
      line_count++;
      tok = extract_level(line_buffer);
      if (tok == NULL) {
         line_nc++;
         continue;
      }
      switch(tok[0]) {
         case 'D':
            line_d++;
            break;
         case 'V':
            line_v++;
            break;
         case 'I':
            line_i++;
            break;
         case 'E':
            line_e++;
            break;
         case 'W':
            line_w++;
            break;
         default:
            line_unk++;
            fprintf(stdout, "[%s]", tok);
      }
   }

   fprintf(stdout, "total number of lines: %d\n", line_count);
   fprintf(stdout, "number of non comforming lines: %d\n", line_nc);
   fprintf(stdout, "number of D level logs: %d\n", line_d);
   fprintf(stdout, "number of V level logs: %d\n", line_v);
   fprintf(stdout, "number of I level logs: %d\n", line_i);
   fprintf(stdout, "number of W level logs: %d\n", line_w);
   fprintf(stdout, "number of E level logs: %d\n", line_e);
   fprintf(stdout, "number of unknown level logs: %d\n", line_unk);

   fclose(fp);
   free(line_buffer);

   return EXIT_SUCCESS;

exit_close:
   fclose(fp);

   return EXIT_FAILURE;
}
