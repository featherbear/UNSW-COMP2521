typedef char *Item;
typedef char *Key;
typedef char *Value;

#define NULLitem NULL

#define key(A) A
#define value(A) A

int cmp(Key, Key);
#define less(k1, k2) (cmp(k1, k2) < 0)
#define eq(k1, k2) (cmp(k1, k2) == 0)

#define eq(k1, k2) (cmp(k1, k2) == 0)
#define less(k1, k2) (cmp(k1, k2) < 0)
#define greater(k1, k2) (cmp(k1, k2) > 0)

void item_show(Item it);
void item_drop(Item it);
