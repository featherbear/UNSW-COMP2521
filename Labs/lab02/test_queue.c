////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 -- Test a Queue ADT implementation.
//
// 2018-12-01	Jashank Jeremy <jashankj@cse.unsw.edu.au>
// YYYY-mm-dd	Your Name Here <zNNNNNNN@student.unsw.edu.au>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "item.h"
#include "queue.h"
#include "testable.h"

int main (void)
{
	white_box_tests ();

	{
		// size check
		Queue q = queue_new();

		assert(queue_size(q) == 0);
		queue_en(q, 5);
		assert(queue_size(q) == 1);

		queue_drop(q);
	}

	{
		// size check and dequeue value
		Queue q = queue_new();
		assert(queue_size(q) == 0);

		queue_en(q, 5);
		assert(queue_size(q) == 1);

		assert(queue_de(q) == 5);
		assert(queue_size(q) == 0);

		queue_drop(q);
	}

	{
		// size check and dequeue values
		Queue q = queue_new();
		assert(queue_size(q) == 0);

		queue_en(q, 5);
		assert(queue_size(q) == 1);

		queue_en(q, 2);
		assert(queue_size(q) == 2);

		queue_en(q, 0);
		assert(queue_size(q) == 3);

		assert(queue_de(q) == 5);
		assert(queue_size(q) == 2);
		assert(queue_de(q) == 2);
		assert(queue_size(q) == 1);
		assert(queue_de(q) == 0);
		assert(queue_size(q) == 0);


		queue_drop(q);
	}

	{
		// queue lots
		Queue q = queue_new();
		assert(queue_size(q) == 0);

		queue_en(q, 5);
		queue_en(q, 5);
		queue_en(q, 5);
		queue_en(q, 5);
		queue_en(q, 5);
		queue_en(q, 5);
		queue_en(q, 5);
		queue_en(q, 5);
		queue_en(q, 5);
		queue_en(q, 5);

		assert(queue_size(q) == 10);

		queue_drop(q);
	}

//	{
//		// dequeue when empty
//		Queue q = queue_new();
//		assert(queue_size(q) == 0);
//		queue_de(q);
//		assert(queue_size(q) == 0);
//		// help what do do if it aborts
//		queue_drop(q);
//	}

	puts ("\nAll tests passed. You are awesome!");
	return EXIT_SUCCESS;
}
