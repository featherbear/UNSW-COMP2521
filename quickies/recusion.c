//
// Created by Jennifer on 11/12/2018.
//

bool all_even(int a[], size_t l, size_t r)
{
    if (a == NULL) return NULL;
    int half = (l + r)/2;

//    //case 0: chcek if l > r --> empty
//    case 1: check if l == r
//    case 2: check that everything from left to right is even

//Binary search trees
//    > prefix (NLR): Node first, then left then right
//    > postfix (LRN): Left, right and then node
//    > infix (LNR): Left node, right
//    > level order: Starting from level 0 to bottom


typedef struct btree_node btree_node;

struct btree_node {
    Item item;
    btree_node *left;
    btree_node *right;
};

int int_btree_sum(btree_node *tree)
{
   if (tree == NULL) return 0;
   return tree->item + int_btree_sum(tree->left) + int_btree_sum(tree->right);
}

bool btree_search_inter (btree_node *tree, Item key)
{

}

bool btree_search_rec (btree-node *tree, Item key)
{
    if (tree == NULL) return false;
    Item cmp = tree->item;
    if (cmp == key) return true;
    else if (cmp > key) return btree_search_rec (tree->left, key);
    else if (cmp < key) return btree_search_rec (tree->right, key);
    return false;
}


