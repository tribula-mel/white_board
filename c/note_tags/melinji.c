#include <stdbool.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "objects.h"

#define MAX_STRING_SIZE (512)

static struct tags_object *tags_head = NULL;
static file_object_t *files_head = NULL;
static struct tags_object *current_tag = NULL;
static file_object_t *current_file = NULL;

static int list_file_add (char *file_name);
static int dir_walk (char *dir_name);
static int list_tag_add (char *file_name);
static int tag_find (char *file_name, char *forest, char *tree);
static int files_search (void);

static int list_tag_add (char *tag_name)
{
   tags_object_t *tag = NULL;

   tag = malloc (sizeof (tags_object_t));
   if (NULL == tag)
   {
      fprintf (stderr, "error: line %d malloc() %s\n",
               __LINE__, strerror (errno));
      return (EXIT_FAILURE);
   }
   memset (tag, 0, sizeof (tags_object_t));

   memcpy (tag -> tag_name, tag_name, (MAX_TAG_NAME_SIZE - 1));
   current_tag -> next = tag;
   current_tag = tag;

   return (EXIT_SUCCESS);
}

static int list_file_add (char *file_name)
{
   file_object_t *file = NULL;

   file = malloc (sizeof (file_object_t));
   if (NULL == file)
   {
      fprintf (stderr, "error: line %d malloc() %s\n",
               __LINE__, strerror (errno));
      return (EXIT_FAILURE);
   }
   memset (file, 0, sizeof (file_object_t));

   memcpy (file -> file_name, file_name, (MAX_FILE_NAME_SIZE - 1));
   current_file -> next = file;
   current_file = file;

   return (EXIT_SUCCESS);
}

static int dir_walk (char *dir_name)
{
   DIR *stream = NULL;
   struct dirent *entity = NULL;
   char full_path[MAX_STRING_SIZE] = {0};
   int rc = -1;

   if (NULL == dir_name)
   {
      fprintf (stderr, "fool: error no name\n");
      return (EXIT_FAILURE);
   }

   // debug printf ("%s: %s\n", __func__, dir_name);

   stream = opendir (dir_name);
   if (NULL == stream)
   {
      fprintf (stderr, "opendir error %s\n", strerror (errno));
      return (EXIT_FAILURE);
   }

   for ( ; ; )
   {
      // so we can check for error vs end of stream
      errno = 0;
      entity = readdir (stream);
      if (NULL == entity)
      {
         if (0 == errno)
            // the end of the stream
            return (EXIT_SUCCESS);
         else
         {
            // error case
            fprintf (stderr, "readdir error %s\n", strerror (errno));
            return (EXIT_FAILURE);      
         }
      }

      switch (entity -> d_type)
      {
         case DT_BLK:
         case DT_CHR:
         case DT_FIFO:
         case DT_LNK:
         case DT_SOCK:
         case DT_UNKNOWN:

            // don't process the above types
            continue;

         case DT_DIR:

            if (0 == strcmp (entity -> d_name, ".") ||
                0 == strcmp (entity -> d_name, ".."))
               // ignore the two
               continue;

            snprintf (full_path, MAX_STRING_SIZE - 1, "%s/%s",
                      dir_name, entity -> d_name);
            // recurse into the new directory
            if (EXIT_FAILURE == dir_walk (full_path))
            {
               fprintf (stderr, "dir_walk failure\n");
               return (EXIT_FAILURE);
            }

            continue;

         case DT_REG:

            // regular file
            // debug printf ("file name: %s\n", entity -> d_name);
            if (0 != strcmp (entity -> d_name, "notes"))
               // we are only interested in files named notes
               continue;

            snprintf (full_path, MAX_STRING_SIZE - 1, "%s/%s",
                      dir_name, entity -> d_name);

            // add to the list
            rc = list_file_add (full_path);
            if (EXIT_FAILURE == rc)
            {
               fprintf (stderr, "error: %s: list_file_add\n", __func__);
               return (EXIT_FAILURE);
            }

            break;

         // no default case
      }
   }

   return (EXIT_SUCCESS);
}

static int tag_find (char *file_name, char *forest, char *tree)
{
   char *found = NULL;
   int tags_fd = -1;

   // printf ("forest %s tree %s\n", forest, tree);

   found = strstr (forest, tree);
   if (NULL == found)
   {
      // string not found
      printf ("string not found\n");
      return (EXIT_FAILURE);
   }

   // printf ("tag %s found in %s\n", tree, forest);

   // output to the tags file

   tags_fd = open ("tags", O_CREAT | O_APPEND | O_WRONLY,
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
   if (-1 == tags_fd)
   {
      fprintf (stderr, "error: line %d open() %s\n",
               __LINE__, strerror (errno));
      return (EXIT_FAILURE);
   }

   // first we have tag itself
   write (tags_fd, tree, strlen (tree));
   // tab
   write (tags_fd, "\t", strlen ("\t"));
   // file
   write (tags_fd, file_name, strlen (file_name));
   // tab
   write (tags_fd, "\t", strlen ("\t"));
   // context
   write (tags_fd, "/", strlen ("/"));
#if 0
   for ( ; (*forest < '0' && *forest > '9') ||
           (*forest < 'a' && *forest > 'z') ||
           (*forest < 'A' && *forest > 'Z') ||
           (*forest == ' '); forest++) ; // move the pointer to the first text
#endif
   for ( ; forest != (found - 1); forest++)
      write (tags_fd, forest, 1); 
   write (tags_fd, "/;\"", strlen ("/;\""));
   write (tags_fd, "\n", strlen ("\n"));

   close (tags_fd);

   return (EXIT_SUCCESS);
}

static int files_search (void)
{
   FILE *content = NULL;
   char *input;
   size_t size = MAX_TAG_NAME_SIZE;
   int rc = -1;
   bool end_of_file = false;

   input = malloc (size);
   if (NULL == input)
   {
      fprintf (stderr, "error: line %d malloc() %s\n",
               __LINE__, strerror (errno));
      return (EXIT_FAILURE);
   }

   // cycle through all the found notes files
   current_file = files_head -> next;
   for ( ; ; )
   {
      printf ("file: %s\n", current_file -> file_name);
      content = fopen (current_file -> file_name, "r");
      if (NULL == content)
      {
         fprintf (stderr, "error: line %d fopen() %s\n",
                  __LINE__, strerror (errno));
         return (EXIT_FAILURE);
      }

next_line:
      memset (input, 0, MAX_TAG_NAME_SIZE);

      errno = 0;
      rc = getline (&input, &size, content);
      if ((rc == -1) && (errno != 0))
      {
         fprintf (stderr, "error: line %d getline() %s\n",
                  __LINE__, strerror (errno));
         return (EXIT_FAILURE);
      }
      if ((rc == -1) && (errno == 0))
      {
         // end of file
         printf ("getline end of file\n");
         end_of_file = true;
      }

      // inner loop goes through all the tags
      current_tag = tags_head -> next;
      for ( ; ; )
      {
         if (true == end_of_file)
            break;

         printf ("input: %s", input);
         printf ("tag: %s\n", current_tag -> tag_name);

         tag_find (current_file -> file_name, input, current_tag -> tag_name);

         if (NULL == current_tag -> next)
            break;

         current_tag = current_tag -> next;
      }

      if (false == end_of_file)
         goto next_line;

//next_file:
      fclose (content);
      end_of_file = false;

      if (NULL == current_file -> next)
         break;

      current_file = current_file -> next;
   }

   return (EXIT_SUCCESS);
}

int main (int argc, char *argv[])
{
   FILE *tag_stream = NULL;
   int opt = -1;
   int rc = -1;
   char input[MAX_TAG_NAME_SIZE];
   char dir_path[MAX_STRING_SIZE] = {0};

   while ((opt = getopt (argc, argv, "t:d:")) != -1)
   {
      switch (opt)
      {
         case 't':
            tag_stream = fopen (optarg, "r");
            if (NULL == tag_stream)
            {
               fprintf (stderr, "error: line %d fopen() %s\n",
                        __LINE__, strerror (errno));
               exit (EXIT_FAILURE);
            }
            break;
         case 'd':
            if (strlen (optarg) > (MAX_STRING_SIZE - 1))
            {
               fprintf (stderr, "error: -d option input too long\n");
               exit (EXIT_FAILURE);
            }
            memcpy (dir_path, optarg, (MAX_STRING_SIZE - 1));
            break;
         default: /* '?' */
            fprintf (stderr, "Usage: %s [-t tags_file] [-d path]\n", argv[0]);
            exit (EXIT_FAILURE);
      }
   }

   if (NULL == tag_stream)
   {
      fprintf (stderr, "Usage: %s [-t tags_file] [-d path]\n", argv[0]);
      exit (EXIT_FAILURE);
   }

   tags_head = malloc (sizeof (struct tags_object));
   if (NULL == tags_head)
   {
      fprintf (stderr, "error: malloc() %s\n", strerror (errno));
      exit (EXIT_FAILURE);
   }
   memset (tags_head, 0, sizeof (struct tags_object));
   current_tag = tags_head;

   files_head = malloc (sizeof (file_object_t));
   if (NULL == files_head)
   {
      fprintf (stderr, "error: malloc() %s\n", strerror (errno));
      exit (EXIT_FAILURE);
   }
   memset (files_head, 0, sizeof (file_object_t));
   current_file = files_head;

   // create a tag list
   for ( ; ; )
   {
      memset (input, 0, MAX_TAG_NAME_SIZE);

      rc = fscanf (tag_stream, "%s", input);
      if (EOF == rc)
         break;

      // debug printf("tag: %s\n", input);
      // add to the list
      rc = list_tag_add (input);
      if (EXIT_FAILURE == rc)
      {
         fprintf (stderr, "error: %s: list_tag_add\n", __func__);
         exit (EXIT_FAILURE);
      }
   }

   // create a notes file list
   rc = dir_walk (dir_path);
   if (EXIT_FAILURE == rc)
   {
      fprintf (stderr, "error: %s: dir_walk\n", __func__);
      exit (EXIT_FAILURE);
   }

#if 0 // debug
   current_tag = tags_head -> next;
   for ( ; ; )
   {
      printf ("tag: %s\n", current_tag -> tag_name);
      if (NULL == current_tag -> next)
         break;
      current_tag = current_tag -> next;
   }

   current_file = files_head -> next;
   for ( ; ; )
   {
      printf ("file: %s\n", current_file -> file_name);
      if (NULL == current_file -> next)
         break;
      current_file = current_file -> next;
   }
#endif

   rc = files_search ();
   if (EXIT_FAILURE == rc)
   {
      fprintf (stderr, "error: %s: files_search\n", __func__);
      exit (EXIT_FAILURE);
   }

   exit (EXIT_SUCCESS);
}
