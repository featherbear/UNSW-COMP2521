typedef int Item;
typedef struct node node;
struct node {
	Item item;
	node *next;
};

void list_drop (node *head)
{
	node *curr = head;
	while (curr != NULL) {
		node *tmp = curr->next;
		free (curr);
		curr = tmp;
	}
}

void list_drop (node *curr)
{
	if (curr == NULL) return;
	list_drop (curr->next);
	free (curr);
}

// [ 1, 2, 3, 4 ]
//            ^
//         ^~~~
//      ^~~~~~~
//   ^~~~~~~~~~

node *list_delete_node (node *curr, node *it)
{
	if (curr == NULL) return NULL;
	if (curr == it) {
		node *tmp = curr->next;
		free (curr);
		return tmp;
	}
	curr->next = list_delete_node (curr->next, it);
	return curr;
}
