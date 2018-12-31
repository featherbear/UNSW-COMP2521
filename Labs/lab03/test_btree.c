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

//    assert(!even_p(5) && odd_p(5) && !negative_p(5));

    Item it1 = int_item_new(5);
    Item it2 = int_item_new(-2);

    BTreeNode t = btree_insert(NULL, it1);
    {
        assert(btree_size_leaf(t) == 1);
        assert(btree_height(t) == 1);
        Item retVal = btree_node_value(t);
        assert(item_cmp(it1, retVal) == 0);
        assert(!even_p(retVal) && odd_p(retVal) && !negative_p(retVal));
        assert(btree_size(t) == 1);

        assert(btree_search(t, it2) == NULL);
    }

    btree_insert(t, it2);
    {
        assert(btree_size_leaf(t) == 1);
        assert(btree_height(t) == 2);

        Item retVal = btree_node_value(t);
        assert(item_cmp(it2, retVal) == 0);
        assert(even_p(retVal) && !odd_p(retVal) && negative_p(retVal));

        assert(btree_size(t) == 2);
        assert(btree_search(t, it2) != NULL);
    }

    t = btree_delete_node(t, it2);
    {
        assert(btree_size_leaf(t) == 1);
        assert(btree_height(t) == 1);
        Item retVal = btree_node_value(t);
        assert(item_cmp(it1, retVal) == 0);
        assert(!even_p(retVal) && odd_p(retVal) && !negative_p(retVal));
        assert(btree_size(t) == 1);

        assert(btree_search(t, it2) == NULL);
    }


    btree_drop(t);
    // BTreeNode *btree_traverse(BTreeNode tree, btree_traversal how, btree_visitor_fp visit);
    // size_t btree_count_if(BTreeNode tree, btree_pred_fp pred);

//    btree_count_if(t, btree_pred_fp)

    puts("\nAll tests passed. You are awesome!");
    return EXIT_SUCCESS;
}

