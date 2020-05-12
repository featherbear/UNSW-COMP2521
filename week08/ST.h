typedef struct st * ST;

ST STinit (int);    /* new symbol table */
int STcount (ST);      /* number of items in table */
void STinsert (ST,Item); /* insert an item */
Item STsearch (ST,Key);  /* find item with given key */
void STdelete (ST,Item); /* delete given item */
Item STselect (ST,int);  /* find nth item */
void STsort (ST,void (*visit)(Item)); /* traverse keys in order */

void show(ST); //for debugging
