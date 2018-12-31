////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 -- Test a binary search tree implementation.
//
// 2018-12-08	Jashank Jeremy <jashankj@cse.unsw.edu.au>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "item_int.h"
#include "btree.h"
#include "testable.h"

#define O() printf(" [OK]\n");
#define T(m) printf("\n"); t(m);
#define t(m) printf(m); printf(" ::\n");
#define A(m) printf("  "); printf(m);

static int sum = 0;
static void node_add (BTreeNode node) {
    sum += int_item(btree_node_value(node));
}

int main(void) {
    white_box_tests();

    typedef struct btree_node *BTreeNode;
    typedef void (*btree_visitor_fp)(BTreeNode);
    typedef bool (*btree_pred_fp)(Item);
    typedef enum btree_traversal {
        BTREE_TRAVERSE_PREFIX,      /**< Order: node, left, right */
        BTREE_TRAVERSE_INFIX,       /**< Order: left, node, right */
        BTREE_TRAVERSE_POSTFIX,     /**< Order: left, right, node */
        BTREE_TRAVERSE_LEVEL,       /**< Order: left-to-right by level */
    } btree_traversal;

    BTreeNode tree;
    {
        T("Testing `btree_size_leaf()`");
        A("Insert int_item 5 into a new BTree");
        {
            // `btree_node_new` not exposed, so create through `btree_insert`
            BTreeNode node = tree = btree_insert(NULL, int_item_new(5));
            assert(node != NULL);
            assert(tree != NULL);
            assert(btree_size(tree) == 1);
            assert(btree_size_leaf(tree) == 1);
        }
        O();

        A("Insert int_item -2 into a existing BTree");
        {
            btree_insert(tree, int_item_new(-2));
            assert(btree_size(tree) == 2);
            assert(btree_size_leaf(tree) == 1);
        }
        O();

        A("Insert int_item 6 into a existing BTree");
        {
            btree_insert(tree, int_item_new(6));
            assert(btree_size(tree) == 3);
            assert(btree_size_leaf(tree) == 2);
        }
        O();

        A("Insert int_item 0 into a existing BTree");
        {
            btree_insert(tree, int_item_new(0));
            assert(btree_size(tree) == 4);
            assert(btree_size_leaf(tree) == 2);
        }
        O();


        A("Insert int_item 3 into a existing BTree");
        {
            btree_insert(tree, int_item_new(3));
            assert(btree_size(tree) == 5);
            assert(btree_size_leaf(tree) == 2);
        }
        O();


        A("Insert int_item -7 into a existing BTree");
        {
            btree_insert(tree, int_item_new(-7));
            assert(btree_size(tree) == 6);
            assert(btree_size_leaf(tree) == 3);
        }
        O();
    }

    {
        T("Testing `btree_traverse_level()` via `btree_traverse()`");
        {
            A("Default visitor function");
            BTreeNode *nodes = btree_traverse(tree, BTREE_TRAVERSE_LEVEL, NULL);

            assert(int_item(btree_node_value(nodes[0])) == 5);
            assert(int_item(btree_node_value(nodes[1])) == -2);
            assert(int_item(btree_node_value(nodes[2])) == 6);
            assert(int_item(btree_node_value(nodes[3])) == -7);
            assert(int_item(btree_node_value(nodes[4])) == 0);
            assert(int_item(btree_node_value(nodes[5])) == 3);
            free(nodes);
            O();
        }
        {
            A("Custom visitor function");
            BTreeNode *shouldBeNull = btree_traverse(tree, BTREE_TRAVERSE_LEVEL, node_add);
            assert(shouldBeNull == NULL);
            assert(sum == 5);
            O();
        }
    }

    {
        T("Testing `even_p()`, `odd_p()` and `negative_p()`");
        Item it;
        {
            t("  int_item_new(5)");
            it = int_item_new(5);
            A("  even_p"); assert(even_p(it) == 0); O();
            A("  odd_p"); assert(odd_p(it) == 1); O();
            A("  negative_p"); assert(negative_p(it) == 0); O();
            item_drop(it);
        }
        {
            T("  int_item_new(2)");
            it = int_item_new(2);
            A("  even_p"); assert(even_p(it) == 1); O();
            A("  odd_p"); assert(odd_p(it) == 0); O();
            A("  negative_p"); assert(negative_p(it) == 0); O();
            item_drop(it);
        }
        {
            T("  int_item_new(0)");
            it = int_item_new(0);
            A("  even_p"); assert(even_p(it) == 1); O();
            A("  odd_p"); assert(odd_p(it) == 0); O();
            A("  negative_p"); assert(negative_p(it) == 0); O();
            item_drop(it);
        }
        {
            T("  int_item_new(-6)");
            it = int_item_new(-6);
            A("  even_p"); assert(even_p(it) == 1); O();
            A("  odd_p"); assert(odd_p(it) == 0); O();
            A("  negative_p"); assert(negative_p(it) == 1); O();
            item_drop(it);
        }
        {
            T("  int_item_new(-7)");
            it = int_item_new(-7);
            A("  even_p"); assert(even_p(it) == 0); O();
            A("  odd_p"); assert(odd_p(it) == 1); O();
            A("  negative_p"); assert(negative_p(it) == 1); O();
            item_drop(it);
        }
    }

    {
        T("Testing `btree_count_if()`");
        A("even_p"); assert(btree_count_if(tree, even_p) == 3); O();
        A("odd_p"); assert(btree_count_if(tree, odd_p) == 3); O();
        A("negative_p"); assert(btree_count_if(tree, negative_p) == 2); O();
    }

    btree_drop(tree);
    puts("\nAll tests passed. You are awesome!");

    return EXIT_SUCCESS;
}

