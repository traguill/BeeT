#include "beet_std.h"

// Dequeue ----------------------------------------------
void push_back(dequeue* deq, void* item)
{
	// Not enough space. Expand the dequeue and reallocate
	if (deq->backId == deq->maxSize - 1 || deq->maxSize == 0)
	{
		deq->maxSize += 8;
		deq->data = (char*)BEET_realloc(deq->data, deq->maxSize);
	}

	memcpy(&deq->data[deq->backId], item, deq->itemSize);
	deq->backId++;
	deq->size++;
}

void push_front(dequeue* deq, void* item)
{
	// Not enough space. Expand the dequeue and reallocate
	if (deq->frontId == -1)
	{
		deq->frontId = 7;
		deq->backId += deq->frontId;
		unsigned int prevSize = deq->maxSize;
		deq->maxSize += deq->frontId + 1;
		char* tmp = (char*)BEET_malloc(deq->maxSize);
		memcpy(tmp + deq->frontId, deq->data, prevSize);
		char* tmp2 = deq->data;
		deq->data = tmp;
		BEET_free(*tmp2);
	}
	memcpy(&deq->data[deq->frontId], item, deq->itemSize);
	deq->frontId--;
	deq->size++;
}

void pop_back(dequeue* deq)
{
	if (deq->size != 0)
	{
		deq->size--;
		deq->backId--;
	}
}

void pop_front(dequeue* deq)
{
	if (deq->size)
	{
		deq->size--;
		deq->frontId++;
	}
}

void* back(dequeue* deq)
{
	return deq->data[deq->backId - 1];
}

void* front(dequeue* deq)
{
	return deq->data[deq->frontId + 1];
}

dequeue * InitDequeue(unsigned int itemSize)
{
	dequeue* deq = (dequeue*)BEET_malloc(sizeof(dequeue));
	deq->itemSize = itemSize;

	deq->data = NULL;
	deq->maxSize = deq->size = 0;
	deq->frontId = -1;
	deq->backId = 0;

	deq->push_back = &push_back;
	deq->push_front = &push_front;
	deq->pop_back = &pop_back;
	deq->pop_front = &pop_front;
	deq->back = &back;
	deq->front = &front;

	return deq;
}
void DestroyDequeue(dequeue * deq)
{
	if (deq != NULL)
	{
		if(deq->data != NULL)
			BEET_free(deq->data);
		BEET_free(deq);
	}
	return;
}
// -----------------------------------------------------------------