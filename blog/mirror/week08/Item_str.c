#include <stdio.h>
#include <stdlib.h>
#include "Item.h"
#include <string.h>

/* read from stdin */
int ITEMscan (Item *item) {
  Item it = malloc (sizeof (*it));
  *item = it;
  if ((scanf ("%d", &(it->keyval)) == 1)){
     strcpy(it->value,"Nothing");
    return 1;
  } else {
    return 0;
  }
}  
void ITEMshow (Item item){
  printf ("%d \t %s \n", item->keyval, item->value);
}
