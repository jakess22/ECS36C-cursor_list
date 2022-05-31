#include "cursor_list.hpp"

#include <iostream>
#include <stdbool.h>

#define NUM_SLOTS 30

struct Slot
{
    int data;
    unsigned next;  // index of next slot
};

static struct Slot SLOTS[NUM_SLOTS];
bool slotAlloc_called = false;

/**
 * "Allocates" a slot that is not currently allocated and returns its index.
 * The very first time that this function is called, the "free list" must be
 * set up, since -- initially -- all of the slots' next members are 0.
 *
 * Does NOT perform any real dynamic memory allocation; malloc(), realloc(),
 * and new are prohibited!
 * Selects the first available node on the "free list", i.e. SLOTS[0].next.
 *
 * The autograder will not check if the next member of the allocated node is 0.
 *
 * Throws std::runtime_error exception if no more available slots.
 */
unsigned slotAlloc()
{
	if (slotAlloc_called == false) {
		for (int i = 0; i < NUM_SLOTS; i++) {
			if (i < NUM_SLOTS - 1) {
				SLOTS[i].next = i + 1;
			} else {
				SLOTS[i].next = 0;
			}
		}
		slotAlloc_called = true;
	}


	//try {
		if (SLOTS[0].next == 0) {
			throw std::runtime_error("slotAlloc(): No more available slots");
		} else {
			unsigned next_buff;
			next_buff = SLOTS[0].next;
			unsigned next_next_buff = SLOTS[next_buff].next;
			if (next_buff == NUM_SLOTS - 1) {
				SLOTS[next_buff].next = 0;
			}
			SLOTS[0].next = next_next_buff;
			return next_buff;
		}
	 //catch (const std::runtime_error &e) {
		//std::cerr << e.what() << std::endl;
		//exit(-1);
	//}


}

/**
 * "Deallocates" the slot/node that is at @index.
 * Does NOT perform any real dynamic memory deallocation; free() and delete
 * are prohibited!
 * Puts the "deallocated" node at the front of the "free list".
 * Does not need to modify the data member of the "deallocated" node.
 */
void slotFree(unsigned index)
{
  unsigned last_freelist = 0;
	unsigned this_freelist = SLOTS[0].next;
	unsigned index_next_buff = SLOTS[index].next;

	while (this_freelist < index) {
		last_freelist = this_freelist;
		this_freelist = SLOTS[this_freelist].next;
	}
	SLOTS[last_freelist].next = index;
	SLOTS[index].next = this_freelist;

	unsigned pos = 1;
	unsigned next = SLOTS[pos].next;
	while (next < index) {
		pos++;
	}

	SLOTS[pos].next = index_next_buff;

}

void printSlots(unsigned start, unsigned end)
{
    for (unsigned i = start; i < end; ++i)
        std::cout << "SLOTS[" << i << "]: " << SLOTS[i].data
            << ' ' << SLOTS[i].next << '\n';
}

/**
 * Returns a new empty list containing just a header node
 * (i.e. returns the index of this header node).
 * Since ths header node would be the last node in this empty list,
 * its next member must be 0.
 *
 * Throws std::runtime_error exception if no more available slots.
 */
unsigned clCreate()
{
	unsigned header_index = slotAlloc();
  SLOTS[header_index].next = 0;
	SLOTS[header_index].data = 0;
	return header_index;
}

/**
 * "Deallocates" all slots in the list that starts at @head, i.e. adds all
 * nodes in this list to the "free list". */

void clDestroy(unsigned head) // output differs from his
{
	unsigned last_node = head;
	unsigned this_node = SLOTS[last_node].next;


	unsigned last_freelist = 0;
	unsigned this_freelist = SLOTS[0].next;
	unsigned last_free_buff = this_freelist;


	while (this_node != 0) {
		SLOTS[last_node].next = SLOTS[this_node].next;

			//adjust freelist
			while (this_freelist < this_node) {
				last_freelist = this_freelist;
				this_freelist = SLOTS[last_freelist].next;
			}
			SLOTS[last_freelist].next = last_node;
			SLOTS[last_node].next = this_freelist;

			last_freelist = last_node;
			this_freelist = this_node;

			last_node = this_node;
			this_node = SLOTS[this_node].next;
	}

	SLOTS[last_node].next = SLOTS[this_node].next;

	//adjust freelist
	while (this_freelist < this_node) {
		last_freelist = this_freelist;
		this_freelist = SLOTS[last_freelist].next;
	}
	SLOTS[last_freelist].next = last_node;
	SLOTS[last_node].next = last_free_buff;
}

/*


void clDestroy(unsigned head) {
	unsigned last_node_buff = head;
	unsigned this_node_buff = SLOTS[last_node_buff].next;

	unsigned last_node = last_node_buff;
	unsigned this_node = this_node_buff;

	unsigned last_freelist = 0;
	unsigned this_freelist = SLOTS[0].next;
	unsigned freelist_buff = this_freelist;

	while (SLOTS[head].next != 0) {
		last_node = last_node_buff;
		this_node = this_node_buff;

		last_freelist = 0;
		this_freelist = SLOTS[0].next;

		while (SLOTS[this_node].next != 0) {
			last_node = this_node;
			this_node = SLOTS[this_node].next;
		}
		SLOTS[last_node].next = 0;
		SLOTS[0].next = last_node;

	}
	//std::cout << last_node << ' ' << this_node << std::endl;
	//std::cout << last_freelist << ' ' << this_freelist << std::endl;
	SLOTS[0].next = this_freelist;
	SLOTS[SLOTS[this_freelist].next].next = this_node;
	SLOTS[last_node].next = freelist_buff;
	SLOTS[this_node].next = last_node;
} */


/**
 * Adds a new node with value @val to the end of the list that contains @node.
 * (@node need not be the index of a header node.)
 * Throws std::runtime_error exception (and should not attempt to append)
 * if no more available slots.
 */
void clAppend(const unsigned node, const int val)
{
  //try {
		if (SLOTS[0].next == 0) {
			throw std::runtime_error("clAppend(): No more available slots");
		} else {
			unsigned append_index = slotAlloc();
			SLOTS[append_index].data = val;
			SLOTS[append_index].next = 0;
			unsigned node_buff = node;
			while (SLOTS[node_buff].next != 0) {
				node_buff = SLOTS[node_buff].next;
			}
			SLOTS[node_buff].next = append_index;
		}
	//} catch (const std::runtime_error &e) {
		//std::cerr << e.what() << std::endl;
		//exit(-1);
	//}


}

/**
 * Adds a new node with value @val immediately after @node.
 * (@node need not be the index of a header node.)
 * Throws std::runtime_error exception (and should not attempt to insert)
 * if no more available slots.
 */
void clInsert(unsigned node, int val)
{
	//try {
		if (SLOTS[0].next == 0) {
			throw std::runtime_error("clInsert(): No more available slots");
		} else if ((node < 0) || (node >= NUM_SLOTS)) {
			throw std::runtime_error("clInsert(): node must be between 0 and NUM_SLOTS");
		} else {
			unsigned insert_index = slotAlloc();
			SLOTS[insert_index].data = val;
			SLOTS[insert_index].next = SLOTS[node].next;
			SLOTS[node].next = insert_index;

		}
	//} catch (const std::runtime_error &e) {
		//std::cerr << e.what() << std::endl;
	//}
}

/**
 * Returns slot index of first node that contains @val in the list
 * whose header node is at index @head.
 * Returns 0 if not found.
 */
unsigned clFind(unsigned head, int val)
{
	unsigned node = head;
  while (SLOTS[node].next != 0) {
		if (SLOTS[node].data == val) {
			return node;
		}
		node = SLOTS[node].next;
	}
	if (SLOTS[node].data == val) {
		return node;
	}
	return 0;
}

/**
 * Prints the data member of each node (except for the header node) in the
 * list whose header node is at index @head.
 * See the examples in prog_hw2.pdf for how the output should look.
 */
void clPrint(unsigned head)
{
  unsigned node = head;
	std::cout << "SLOTS[" << node << "]: (header)" << std::endl;
	node = SLOTS[node].next;
	if (clLength(head) > 0) {
		for (unsigned i = 0; i < clLength(head); i++) {
			std::cout << "SLOTS[" << node << "]: " << SLOTS[node].data << std::endl;
			node = SLOTS[node].next;
		}
	}
}


/**
 * Deletes first occurrence of a node that contains @val in the list
 * whose header node is at index @head.
 * Returns true if a node was deleted; returns false otherwise.
 */
bool clDelete(unsigned head, int val)
{
  unsigned last_node = head;
	unsigned this_node = SLOTS[last_node].next;
	while (SLOTS[this_node].next != 0) {
		if (SLOTS[this_node].data == val) {
			SLOTS[last_node].next = SLOTS[this_node].next;

			unsigned last_freelist = 0;
			unsigned this_freelist = SLOTS[0].next;

			//adjust freelist
			while (this_freelist < this_node) {
				last_freelist = this_freelist;
				this_freelist = SLOTS[last_freelist].next;
			}
			SLOTS[last_freelist].next = this_node;
			SLOTS[this_node].next = this_freelist;
			return true;
		}
		last_node = this_node;
		this_node = SLOTS[this_node].next;
	}

	if (SLOTS[this_node].data == val) {
		SLOTS[last_node].next = SLOTS[this_node].next;

		unsigned last_freelist = 0;
		unsigned this_freelist = SLOTS[0].next;

		//adjust freelist
		while (this_freelist < this_node) {
			last_freelist = this_freelist;
			this_freelist = SLOTS[last_freelist].next;
		}
		SLOTS[last_freelist].next = this_node;
		SLOTS[this_node].next = this_freelist;
		return true;
	}

	return false;
}

/**
 * Returns true if the header node that is at index @head is the only
 * node in its list (indicating the list is empty).
 * Otherwise, returns false.
 */
bool clIsEmpty(unsigned head)
{
	if (SLOTS[head].next == 0) {
		return true;
	} else {
		return false;
	}

}

/**
 * Returns the length of the given list, not counting the header node.
 * Assumes that @head is the index of a header node.
 */
unsigned clLength(unsigned head)
{
	unsigned length = 0;
	unsigned pos = head;
	while (SLOTS[pos].next != 0) {
		length++;
		pos = SLOTS[pos].next;
	}
	return length;
}

/**
 * Deletes all nodes in the list whose header node is at index @head
 * that have the value @val.
 * Returns the number of nodes deleted.
 */

//testing: what if last case is zero is deleted
unsigned clDeleteAll(unsigned head, int val)
{
	unsigned count = 0;
  unsigned last_node = head;
  unsigned this_node = SLOTS[last_node].next;


  unsigned last_freelist = 0;
  unsigned this_freelist = SLOTS[0].next;
  unsigned last_free_buff = this_freelist;


  while (this_node != 0) {
 	 if (SLOTS[this_node].data == val) {
 		 count++;
 		 unsigned this_node_buff = this_node;

 		 SLOTS[last_node].next = SLOTS[this_node].next;

 		 last_node = this_node;
 		 this_node = SLOTS[this_node].next;

 		 while (this_freelist < this_node_buff) {
 			 last_freelist = this_freelist;
 			 this_freelist = SLOTS[this_freelist].next;
 		 }

 		 SLOTS[last_freelist].next = this_node_buff;
 		 last_freelist = this_node_buff;



 	 } else {
 		 last_node = this_node;
 		 this_node = SLOTS[this_node].next;
 	 }
  }

  if (SLOTS[this_node].data == val) {
 	 count++;
 	 unsigned this_node_buff = this_node;
 	 SLOTS[last_node].next = SLOTS[this_node].next;

 	 last_node = this_node;
 	 this_node = SLOTS[this_node].next;

 	 while (this_freelist < this_node_buff) {
 		 last_freelist = this_freelist;
 		 this_freelist = SLOTS[this_freelist].next;
 	 }

 	 SLOTS[last_freelist].next = this_node_buff;
 	 last_freelist = this_node_buff;
  }

  SLOTS[last_freelist].next = last_free_buff;
  return count;
}

/**
 * Creates a copy of the list whose header node is at index @head
 * and returns the index of this copy's header node.
 *
 * If there are not enough slots to create a copy, then no slots
 * will be allocated (although the free list, without changing length, might
 * still be modified), and an std::runtime_error exception will be thrown.
 */
unsigned clCopy(unsigned head)
{
	unsigned new_head = clCreate(); // make new list
	unsigned next_copy = 99;


	unsigned this_copy = new_head;
	unsigned this_temp = head;

	while (SLOTS[this_temp].next != 0) {
		next_copy = slotAlloc();
		SLOTS[this_copy].next = next_copy;
		SLOTS[this_copy].data = SLOTS[this_temp].data;

		this_temp = SLOTS[this_temp].next;
		this_copy = next_copy;
	}
	SLOTS[this_copy].next = 0;
	SLOTS[this_copy].data = SLOTS[this_temp].data;


	return new_head;
}
/**
 * Reverses the list whose header node is at index @head.
 *
 * IMPLEMENTATION DETAILS: (not normally in a header file)
 *
 * - You are not allowed to allocate or deallocate any slots in this function
 *   (or to call any function that does this).
 *
 * - Whether you choose to change the nodes' data members or their next
 *   members is your choice. (The autograder won't care.)
 *   Whatever you do, the header node must remain the header node,
 *   i.e. @head should still be the index of the list's header node.
 *   In other words, you are reversing all nodes starting at SLOTS[head].next.
 *
 * OPTIONAL CHALLENGE that is like an interview problem:
 * Implement this function with O(1) auxiliary space, i.e. with a constant
 * amount of local variables/space that doesn't scale with the size of the
 * list. Do this all while having the function run in linear (O(n)) time,
 * where n is the number of nodes.
 */

void clReverse(unsigned head)
{

	unsigned node = SLOTS[head].next;
	//unsigned next_node = SLOTS[node].next;

	unsigned switch_node = SLOTS[head].next;
	unsigned old_switch = 0;

	unsigned temp = 99;

	//while (node < switch_node) {
	for (unsigned i = 0; i < clLength(head) / 2; i++) {
		while (SLOTS[switch_node].next != old_switch) {
			switch_node = SLOTS[switch_node].next;
		}

		temp = SLOTS[switch_node].data;
		SLOTS[switch_node].data = SLOTS[node].data;
		SLOTS[node].data = temp;

		node = SLOTS[node].next;
		old_switch = switch_node;
		switch_node = SLOTS[head].next;
	}



}
