#include "BeeT_dequeue.h"
#include "BeeT_std.h"

// Dequeue BeeT_Node helpers --------------------------------------------------------------
void push_back(dequeue* deq, void* item)
{

}

void push_front(dequeue* deq, void* item)
{

}

void pop_back(dequeue* deq)
{

}

void pop_front(dequeue* deq)
{

}

void* back(dequeue* deq)
{

}

void* front(dequeue* deq)
{

}

dequeue * InitDequeue(int itemSize)
{
	dequeue* deq = (dequeue*)BEET_malloc(sizeof(dequeue));
	deq->itemSize = itemSize;

	deq->data = NULL;
	deq->maxSize = 0;
	deq->size = 0;

	deq->push_back = push_back;
	deq->push_front = push_front;
	deq->pop_back = pop_back;
	deq->pop_front = pop_front;
	deq->back = back;
	deq->front = front;

	return deq;
}
