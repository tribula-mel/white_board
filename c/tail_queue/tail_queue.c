#include <sys/queue.h>

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 2

TAILQ_HEAD(tailhead, entry) head;
//static struct tailhead *headp; /* Tail queue head. */

static short count = 0;
static int ***mel;

typedef struct data {
   int value;
} data_t;

typedef struct entry {
   void *data;

   TAILQ_ENTRY(entry) entries; /* Tail queue. */
} entry_t;

static void clear_queue(void) {
   entry_t *item;
   int count = 0;

   while (!TAILQ_EMPTY(&head)) {
       printf("remove count[%d]\n", ++count);
       item = TAILQ_FIRST(&head);
       item = NULL;
       TAILQ_REMOVE(&head, item, entries);
       free(item);
   }
}

static int insert_q(entry_t *item) {
   if (item == NULL) {
      printf("passed NULL\n");
      return -1;
   }

   if (count == MAX_SIZE) {
      /* first remove the item at head */
      TAILQ_REMOVE(&head, TAILQ_FIRST(&head), entries);
   } else {
      count++;
   }

   /* add to the tail of the queue */
   TAILQ_INSERT_TAIL(&head, item, entries);

   return 0;
}

static entry_t *get_q(void) {
   entry_t *item;

   if (count == 0) {
      /* queue is empty */
      return NULL;
   }

   if (TAILQ_FIRST(&head) != NULL) {
      count--;

      /* return the element at the head */
      item = TAILQ_FIRST(&head);
      /* we can remove it now */
      TAILQ_REMOVE(&head, TAILQ_FIRST(&head), entries);

      return item;
   }

   return NULL;
}

int main(void) {
   entry_t *item_1;
   entry_t *item_2;
   entry_t *item_3;
   entry_t *item_4;
   entry_t *item_5;
   entry_t *np;

   data_t *data_1;
   data_t *data_2;
   data_t *data_3;
   data_t *data_4;
   data_t *data_5;

   TAILQ_INIT(&head); /* Initialize the queue. */

   item_1 = malloc(sizeof(entry_t));
   item_2 = malloc(sizeof(entry_t));
   item_3 = malloc(sizeof(entry_t));
   item_4 = malloc(sizeof(entry_t));
   item_5 = malloc(sizeof(entry_t));

   data_1 = malloc(sizeof(data_t));
   data_2 = malloc(sizeof(data_t));
   data_3 = malloc(sizeof(data_t));
   data_4 = malloc(sizeof(data_t));
   data_5 = malloc(sizeof(data_t));

   data_1->value = 1;
   data_2->value = 2;
   data_3->value = 3;
   data_4->value = 4;
   data_5->value = 5;

   item_1->data = data_1;
   item_2->data = data_2;
   item_3->data = data_3;
   item_4->data = data_4;
   item_5->data = data_5;

   /* case 1 - insert three times no get */
   insert_q(item_1);
   insert_q(item_2);
   insert_q(item_3);
   printf("Case 1 - insert 3 items into the queue, no get\n");
   printf("expected result is values 2 and 3\n");
   for (np = TAILQ_FIRST(&head); np != NULL; np = TAILQ_NEXT(np, entries))
      printf("value(%d)\n", ((data_t *)np->data)->value);

   /* case 2 - do a get */
   printf("\nCase 2 - do one get\n");
   printf("expected result is value 2\n");
   np = get_q();
   if (np != NULL)
      printf("value(%d)\n", ((data_t *)np->data)->value);
   printf("print the rest of the items in the queue\n");
   printf("expected result is value 3\n");
   for (np = TAILQ_FIRST(&head); np != NULL; np = TAILQ_NEXT(np, entries))
      printf("value(%d)\n", ((data_t *)np->data)->value);

   /*  case 3 - do two more gets */
   printf("\nCase 3 - do one get followed by another\n");
   printf("expected result for first get is value 3\n");
   np = get_q();
   if (np != NULL)
      printf("value(%d)\n", ((data_t *)np->data)->value);
   printf("print the rest of the items in the queue\n");
   printf("expected result is that the queue should be empty\n");
   for (np = TAILQ_FIRST(&head); np != NULL; np = TAILQ_NEXT(np, entries))
      printf("value(%d)\n", ((data_t *)np->data)->value);
   printf("do another get\n");
   printf("expected result is NULL\n");
   np = get_q();
   if (np == NULL)
      printf("value(NULL)\n");

   /* case 4 -insert one more element into the queue, no get */
   printf("\nCase 4 - do one insert\n");
   printf("print the rest of the items in the queue\n");
   insert_q(item_4);
   printf("expected result for first get is value 4\n");
   for (np = TAILQ_FIRST(&head); np != NULL; np = TAILQ_NEXT(np, entries))
      printf("value(%d)\n", ((data_t *)np->data)->value);

   /* case 5 -insert one more element into the queue, no get */
   printf("\nCase 5 - do one insert\n");
   printf("print the rest of the items in the queue\n");
   insert_q(item_5);
   printf("expected result for first get is value 4 and 5\n");
   for (np = TAILQ_FIRST(&head); np != NULL; np = TAILQ_NEXT(np, entries))
      printf("value(%d)\n", ((data_t *)np->data)->value);

#if 0
   /* Delete. */
   while (TAILQ_FIRST(&head) != NULL)
      TAILQ_REMOVE(&head, TAILQ_FIRST(&head), entries);
#endif
   clear_queue();

   free(item_1);
   free(item_2);
   free(item_3);
   free(item_4);
   free(item_5);

   free(data_1);
   free(data_2);
   free(data_3);
   free(data_4);
   free(data_5);

   return 0;
}
