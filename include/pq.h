#ifndef __ILRD_PQ_H__
#define __ILRD_PQ_H__

#include <stddef.h> /* size_t */

typedef struct pariroty_queue_t pq_t;

typedef int (*pq_is_match_t) (const void* data1, const void* data2);
typedef int (*pq_compare_t) (const void* data1, const void* data2, const void* param);




/*
	The function creates Priority Queue
	Input:
		pq_compare_t - the comparing function
		void*	     - Param Data
	Output:
		pq_t*	  - pointer to the queue
--------
	Limitations:
		None
	Complexity:
		O(1) + malloc
*/
pq_t* PQCreate(pq_compare_t comparer, void* param);

/*
	The function destroy Priority Queue
	Input:
		pq_t*	- pointer to a pqueue
	Output:
		None
--------
	Limitations:
		None
	Complexity:
		O(1)
*/
void PQDestroy(pq_t* q);

/*
	The function Inserts node.
	Input:
		pq_t*	- pointer to a queue
		void*   - the data
	Output:
		int     - status | 0 => Success
--------
	Limitations:
		None
	Complexity:
		O(n)
*/
int PQEnqueue(pq_t* q, void* data);


/*
	The function Removes node.
	Input:
		pq_t*	- pointer to a queue
	Output:
		None
--------
	Limitations:
		Undefined with an empty Queue
	Complexity:
		O(1)
*/
void PQDequeue(pq_t* q);

/*
	The function returns the size
	Input:
		pq_t*	- pointer to a queue
	Output:
		size_t		- the size
--------
	Limitations:
		None
	Complexity:
		O(n)
*/
size_t PQSize(const pq_t* q);

/*
	The function checks if the queue is empty
	Input:
		pq_t*	- pointer to a queue
	Output:
		int		- 1 for "empty" and 0 for "not empty"
--------
	Limitations:
		None
	Complexity:
		O(1)
*/
int PQIsEmpty(const pq_t* q);

/*
	The function returns the Data from FirstToGo.
	Input:
		pq_t*	- pointer to queue
	Output:
		void*	- The data
--------
	Limitations:
		Undefined with an empty Queue
	Complexity:
		O(1)
*/
void* PQPeek(pq_t* q);

/*
	The function Removes all the nodes.
	Input:
		pq_t*	- pointer to a queue
	Output:
		None
--------
	Limitations:
		None
	Complexity:
		O(n)
*/
void PQClear(pq_t* q);



/*
	The function Removes specific element from queue.
	Input:
		pq_t*		- pointer to a queue
		pq_is_match_t   - the matching func
		void*		- the data to find
	Output:
		void*		- the data inside the node or NULL.
--------
	Limitations:
		None
	Complexity:
		O(n)
*/
void* PQErase(pq_t* q, pq_is_match_t is_match_func, void* data);


#endif
