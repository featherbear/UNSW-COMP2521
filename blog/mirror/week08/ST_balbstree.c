#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "Item.h"
#include "ST.h"

// COMPILE WITH
// gcc -Wall -Werror -O -o ST_balbstree ST_balbstree.c Item_str.c
// ./ST_balbstree


#define NORMAL 0
#define ROOT 1
#define RANDOM 2
#define SPLAY 3

typedef struct node * link;
typedef struct st * ST;


struct st{
    link root;
};


struct node { 
  Item item; 
  link left, right; 
  int size;
};




static link emptyTree = NULL;
//empty tree is the dummy node

link NEW (Item item, link l, link r, int size){ 
  link newLink = malloc (sizeof *newLink); 
  
  newLink->item = item; 
  newLink->left = l; 
  newLink->right = r; 
  newLink->size = size;
  return newLink;
}



//We ignore the MAX
ST STinit (int MAX) { 
    ST s = malloc(sizeof(struct st));
    if(emptyTree == NULL){
       emptyTree = NEW(NULLitem,NULL,NULL,0);
    }
    srand(time(NULL));
    s->root = emptyTree;
    return s;
}


int STcount (ST st)  {
 
  return st->root->size; 
}

Item searchR (link currentLink, Key searchKey) { 
  if (currentLink == emptyTree) {
    return NULLitem;
  }
  Key currentKey = key (currentLink->item);
  if (eq(searchKey, currentKey)) {
    return currentLink->item;
  }
  if (less(searchKey, currentKey)) {
    return searchR (currentLink->left, searchKey);
  } else {
    return searchR (currentLink->right, searchKey);
  }
}

Item STsearch (ST st,Key searchKey) { 
  return searchR (st->root, searchKey); 
} 



link insertR (link currentLink, Item item) { 
  
  if (currentLink == emptyTree) {
    return (NEW (item, emptyTree, emptyTree, 1));
  }
  if (less (key (item), key (currentLink->item))) {
    currentLink->left = insertR (currentLink->left, item);
  } else if (less (key (currentLink->item), key (item))) {
    currentLink->right = insertR (currentLink->right, item);
  }
  currentLink->size++;
  return (currentLink);
}






Item selectR (link currentTree, int k) {
  if (currentTree == emptyTree) {
    return NULLitem;
  }
  if (currentTree->left->size == k) {
    return (currentTree->item);
  }
  
  if (currentTree->left->size > k) {
    return (selectR (currentTree->left, k));
  }

  return (selectR (currentTree->right, k - 1 - currentTree->left->size));
}

Item STselect (ST st,int k) {
  return (selectR (st->root, k));
}

void STdelete (ST st, Item item) {
  // not implemented yet
}


void sortR (link currentLink, void (*visit)(Item)) { 
  if (currentLink == emptyTree) {
    return;
  }
  sortR (currentLink->left, visit);
  visit (currentLink->item); 
  sortR (currentLink->right, visit);
}

void STsort (ST st,void (*visit)(Item)) { 
   sortR (st->root, visit); 
} 

static link rotLeft (link currentTree) {
    if(currentTree == emptyTree || currentTree->right == emptyTree){
        return currentTree;
    }
    link rotLTree = currentTree->right;
    if(rotLTree == emptyTree) return currentTree; 
    currentTree->right = rotLTree->left;
    rotLTree->left = currentTree;
    rotLTree->left->size = rotLTree->left->left->size + rotLTree->left->right->size +1;
    rotLTree->size = rotLTree->left->size + rotLTree->right->size + 1;
   
    return rotLTree;
}

static link rotRight (link currentTree) {
    if(currentTree == emptyTree || currentTree->left == emptyTree){
        return currentTree;
    }
    link rotRTree = currentTree->left;
    if(rotRTree == emptyTree) return currentTree; 
    currentTree->left = rotRTree->right;
    rotRTree->right = currentTree;
    
    rotRTree->right->size = rotRTree->right->left->size + rotRTree->right->right->size + 1;
    rotRTree->size = rotRTree->right->size+ rotRTree->left->size+1;

    return rotRTree;
}
/*

//does not update counter properly
static link rotLeft (link currentTree) {
  if(currentTree == NULL || currentTree == emptyTree) return currentTree;
  link rotLTree = currentTree->right;  
  if(rotLTree == emptyTree) return currentTree; 
  currentTree->right = rotLTree->left;
  rotLTree->left = currentTree;
  return rotLTree;
}

//does not update counter properly
static link rotRight (link currentTree) {
   if(currentTree == NULL || currentTree == emptyTree) return currentTree;
  link rotRTree = currentTree->left; 
  if(rotRTree == emptyTree) return currentTree; 
  currentTree->left = rotRTree->right;
  rotRTree->right = currentTree;
 
  return rotRTree;
}
*/

// partition tree at node with position pos (counting from 0) in the
// sorted sequence of all items, node become new root node.
// Will only work once as rotRight and rotLeft do not 
// update size fields properly
link partitionR (link currentTree, int pos) {
    int leftSubtreeSize = currentTree->left->size;
    if (leftSubtreeSize > pos) {
        currentTree->left = partitionR (currentTree->left, pos);
        currentTree = rotRight (currentTree) ;
    } else if (leftSubtreeSize < pos) {
        currentTree->right = 
        partitionR (currentTree->right, pos - 1 - leftSubtreeSize);
        currentTree = rotLeft (currentTree) ;    
    }
    return currentTree;
}

typedef struct node * Node;
static void showTree(Node  t);
link balance(link tree){
    if(tree->size >=2){
        tree = partitionR(tree,tree->size/2);
        showTree(tree);
        tree->left = balance(tree->left);
       
        tree->right = balance(tree->right);
        
    }
    return tree;
}




link insertAtRoot(link currentLink, Item item){
  if(currentLink == emptyTree){
    return (NEW(item,emptyTree,emptyTree,1));
  } 
  if (less (key (item), key (currentLink->item))) {
    currentLink->left = insertAtRoot (currentLink->left, item);
    currentLink = rotRight(currentLink);
  } else  {
    currentLink->right = insertAtRoot (currentLink->right, item);
    currentLink = rotLeft(currentLink);
  } 
  
  return (currentLink);
  
}

link insertRand(link currentLink, Item item){
 
  if (currentLink == emptyTree) {
    return (NEW (item, emptyTree, emptyTree, 1));
  }
  //Prob 1/N
 
  if(rand() < RAND_MAX/(currentLink->size+1)){
    return insertAtRoot(currentLink,item);
  } else if (less (key (item), key (currentLink->item))) {
    currentLink->left = insertRand (currentLink->left, item);
  } else  {
    currentLink->right = insertRand (currentLink->right, item);
  } 
  currentLink->size++;
  return (currentLink);
}

void dump(link n, int level, int maxLevel){
  if(n == emptyTree){
    printf("X\n");
  } else if (n == NULL){
    //printf("NULL\n");
    } else if (level == maxLevel){
        printf("...\n");
    }else {
        int i;
        printf("%d(%d) ",n->item->keyval,n->size);
       
        printf("\n");
        if (n->left != NULL) {
            for (i = 0; i < level; i++){
                printf("|  ");
            }
            printf("|--L: ");
            dump(n->left, level+1, maxLevel);
        }
        if (n->right != NULL) {
            for (i = 0; i < level; i++){
                printf("|  ");
            }
            printf("|--R: ");
            dump(n->right, level+1, maxLevel);
        }
    }
}


void dumpTree(ST st){
  dump(st->root,0,10);
}


void displayTree(ST st){
    showTree(st->root);
}

link splayInsert (link tree, Item item) {
  Key v = key (item);
  if (tree == emptyTree)
    return (NEW (item, emptyTree, emptyTree, 1)); 
  
  //Item is small and goes on left
  if (less (v, key(tree->item))) {
    if (tree->left == emptyTree) {
        //we do not insert it as the left child then rotate
        //we just insert it in the right spot straight away
      return (NEW (item, emptyTree, tree, tree->size+1));
    }
    if (less (v, key (tree->left->item))) {
      tree->left->left = splayInsert (tree->left->left, item);
      tree = rotRight (tree); 
    } else {
      tree->left->right = splayInsert (tree->left->right, item);
      tree->left = rotLeft (tree->left);
    }
    return rotRight (tree);
  } 
    
    else {
    if (tree->right == emptyTree) {
      return (NEW (item, tree, emptyTree, tree->size+1));
    }
    if (less (key (tree->right->item), v)) {
      tree->right->right = splayInsert (tree->right->right, item);
      tree = rotLeft (tree);
    } else {
      tree->right->left = splayInsert (tree->right->left, item);
      tree->right = rotRight (tree->right);
    }
    return rotLeft (tree);
  }
}
//Global variable
int type = NORMAL;
void STinsert (ST st,Item item) { 
  assert(st != NULL);
  if(STsearch(st,key(item)) == NULLitem){
    
      if(type == NORMAL){
          st->root = insertR(st->root,item);
      } else if (type == ROOT){
		st->root = insertAtRoot(st->root,item);
      } else if (type == RANDOM){
  		st->root = insertRand(st->root,item);
      } else if (type == SPLAY){
 		st->root = splayInsert(st->root,item);
      } else {
		fprintf(stderr,"No such insertion type\n");
		abort();
	  }
  }
}


int main(int argc, char * argv[]){
  ST st = STinit(10);
  
  printf("Checking stcount is 0\n");
  assert(STcount(st) == 0);
   
  int i = 0;
  
  Item currItem;
      
 

  if(argc > 1){
      type = atoi(argv[1]);
  }
  while(ITEMscan(&currItem)){
      //will ignore duplicates
      STinsert(st,currItem);
      //displayTree(st);
      //printf("\n");
      i++;
  }
  
  
  displayTree(st);
  st->root=balance(st->root);
  displayTree(st);
  return 0;
}

//////////The following section contains code to display a tree
/////////This code is very complicated. You do not need to understand these
// functions

// ASCII tree printer
// Courtesy: ponnada
// Via: http://www.openasthra.com/c-tidbits/printing-binary-trees-in-ascii/
    

static int max(int x, int y){
    if(x > y){
        return x;
    }else{
	return y;
    }
}

// data structures
typedef struct asciinode_struct asciinode;
struct asciinode_struct
{
        asciinode * left, * right;
        //length of the edge from this node to its children
        int edge_length;
        int height;
        int lablen;
        //-1=I am left, 0=I am root, 1=I am right
        int parent_dir;
        //max supported unit32 in dec, 10 digits max
        char label[11];
};  



void print_level(asciinode *node, int x, int level);
void compute_edge_lengths(asciinode *node);
void compute_lprofile(asciinode *node, int x, int y);
void compute_rprofile(asciinode *node, int x, int y);
asciinode *build_ascii_tree(Node  t);
void free_ascii_tree(asciinode *node);
        
#define MAX_HEIGHT 1000
int lprofile[MAX_HEIGHT];
int rprofile[MAX_HEIGHT];
#define INFINITY (1<<20)

#define MIN(X,Y)  (((X) < (Y)) ? (X) : (Y))
#define MAX(X,Y)  (((X) > (Y)) ? (X) : (Y))

int gap = 3;  // gap between left and right nodes
        
//used for printing next node in the same level,
//this is the x coordinate of the next char printed
int print_next;

//prints ascii tree for given Tree structure
static void showTree(Node  t)
{
        asciinode *proot;
        int xmin, i;
        if (t == emptyTree) return;
        proot = build_ascii_tree(t);
        compute_edge_lengths(proot);
        for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
                lprofile[i] = INFINITY;
        compute_lprofile(proot, 0, 0);
        xmin = 0;
        for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
                xmin = MIN(xmin, lprofile[i]);
        for (i = 0; i < proot->height; i++) {
                print_next = 0;
                print_level(proot, -xmin, i);
                printf("\n");
        }
        if (proot->height >= MAX_HEIGHT) {
                printf("(Tree is taller than %d; may be drawn incorrectly.)\n",
                        MAX_HEIGHT);
        }
        free_ascii_tree(proot);
}

//This function prints the given level of the given tree, assuming
//that the node has the given x cordinate.
void print_level(asciinode *node, int x, int level)
{
        int i, isleft;
        if (node == NULL) return;
        isleft = (node->parent_dir == -1);
        if (level == 0)
        {
                for (i=0; i<(x-print_next-((node->lablen-isleft)/2)); i++)
                        printf(" ");
                print_next += i;
                printf("%s", node->label);
                print_next += node->lablen;
        }
 else if (node->edge_length >= level)
        {
                if (node->left != NULL)
                {
                  for (i=0; i<(x-print_next-(level)); i++)
                  {
                    printf(" ");
                  }
                  print_next += i;
                  printf("/");
                  print_next++; 
                }
                if (node->right != NULL)   
                {
                  for (i=0; i<(x-print_next+(level)); i++)
                  {
                    printf(" ");
                  }
                  print_next += i;
                  printf("\\");
                  print_next++; 
                }  
        }
    else
        {
                print_level(node->left,
                            x-node->edge_length-1,
                            level-node->edge_length-1);
                print_level(node->right,
                            x+node->edge_length+1,
                            level-node->edge_length-1);
        }
}
                  

//This function fills in the edge_length and
//height fields of the specified tree
void compute_edge_lengths(asciinode *node)
{
        int h, hmin, i, delta;
        if (node == NULL) return;
        compute_edge_lengths(node->left);
        compute_edge_lengths(node->right);
                
        /* first fill in the edge_length of node */
        if (node->right == NULL && node->left == NULL)
                node->edge_length = 0;
 else
        {         
                if (node->left == NULL)
                        hmin = 0;
                else {
                        for (i = 0; i < node->left->height && i < MAX_HEIGHT; i++)
                                rprofile[i] = -INFINITY;
                        compute_rprofile(node->left, 0, 0);
                        hmin = node->left->height;
                }
                if (node->right == NULL)
                        hmin = 0;
                else {
                        for (i=0; i<node->right->height && i < MAX_HEIGHT; i++)
                                lprofile[i] = INFINITY;
                        compute_lprofile(node->right, 0, 0);
                        hmin = MIN(node->right->height, hmin);
                }
		delta = 4;
                for (i=0; i<hmin; i++)
                        delta = max(delta, gap+1+rprofile[i]-lprofile[i]);
                        
                //If the node has two children of height 1, then we allow the
                //two leaves to be within 1, instead of 2
                if (((node->left != NULL && node->left->height == 1) ||
                      (node->right != NULL && node->right->height == 1))
                                && delta > 4)
                        delta--;
                node->edge_length = ((delta+1)/2) - 1; 
        }
                        
        //now fill in the height of node
        h = 1;

	if (node->left != NULL)
                h = MAX(node->left->height + node->edge_length + 1, h);   
        if (node->right != NULL)
                h = MAX(node->right->height + node->edge_length + 1, h);
        node->height = h;
}

asciinode *build_ascii_tree_recursive(Node t)
{
        asciinode * node;
         
        if (t == emptyTree) return NULL;
        node = malloc(sizeof(asciinode));
        node->left = build_ascii_tree_recursive(t->left);
        node->right = build_ascii_tree_recursive(t->right);
        if (node->left != NULL) node->left->parent_dir = -1;
        if (node->right != NULL) node->right->parent_dir = 1;
        sprintf(node->label, "%d (%d)", t->item->keyval,t->size);
        node->lablen = strlen(node->label);;
        return node;
}        

//Copy the tree into the ascii node structre
asciinode *build_ascii_tree(Node  t)
{
        asciinode *node;
        if (t == emptyTree) return NULL;
        node = build_ascii_tree_recursive(t);
        node->parent_dir = 0;
        return node;
}
        
//Free all the nodes of the given tree
void free_ascii_tree(asciinode *node)
{
        if (node == NULL) return;
        free_ascii_tree(node->left);
        free_ascii_tree(node->right);
        free(node);
}

//It assumes that the center of the label of the root of this tree
//is located at a position (x,y).  It assumes that the edge_length
//fields have been computed for this tree.
void compute_lprofile(asciinode *node, int x, int y)
{
        int i, isleft;
        if (node == NULL) return;
        isleft = (node->parent_dir == -1);
        lprofile[y] = MIN(lprofile[y], x-((node->lablen-isleft)/2));
        if (node->left != NULL) {
                for (i=1; i <= node->edge_length && y+i < MAX_HEIGHT; i++)
                        lprofile[y+i] = MIN(lprofile[y+i], x-i);
        }
        compute_lprofile(node->left, x-node->edge_length-1, y+node->edge_length + 1);
        compute_lprofile(node->right, x+node->edge_length+1, y+node->edge_length + 1);
}

void compute_rprofile(asciinode *node, int x, int y)
{
        int i, notleft;
        if (node == NULL) return;
        notleft = (node->parent_dir != -1);
        rprofile[y] = MAX(rprofile[y], x+((node->lablen-notleft)/2));
        if (node->right != NULL) {
                for (i=1; i <= node->edge_length && y+i < MAX_HEIGHT; i++)
                        rprofile[y+i] = MAX(rprofile[y+i], x+i);
        }
        compute_rprofile(node->left, x-node->edge_length-1, y+node->edge_length + 1);
        compute_rprofile(node->right, x+node->edge_length+1, y+node->edge_length + 1);
}


        
