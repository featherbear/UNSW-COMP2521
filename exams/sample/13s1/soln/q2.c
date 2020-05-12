// ListReverse ... reverse a List
void ListReverse(List L)
{
	assert(L != NULL);
	if (L->first == NULL) return;
	Link curr, next;
	curr = L->first;
	L->last = L->first;
	while (curr != NULL){
		next = curr->next;
		curr->next = L->first;
		L->first = curr;
		curr = next;
	}
	L->last->next = NULL;
}
