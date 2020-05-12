typedef int Key;         /* keys type may vary! */
struct record {
  Key  keyval;
  char value[10];        /* value type may vary! */
};
                         /* type of items */
typedef struct record *Item;       

#define key(A)     ((A)->keyval)
#define eq(A, B)   ( A == B )
#define less(A, B) ( A < B )

#define NULLitem  NULL  /* special value for "no item" */

int ITEMscan (Item *);  /* read from stdin */
void ITEMshow (Item);   /* print to stdout */
