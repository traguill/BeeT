#include "beet_std.h"

// -----------------------------------------------------------------------------------------
// Dequeue
// -----------------------------------------------------------------------------------------

struct BeeT_dequeue
{
	struct node_dequeue* head;
	struct node_dequeue* tail;
};

dequeue* InitDequeue()
{
	dequeue* d = BEET_malloc(sizeof(dequeue));
	if (d != NULL)
		d->head = d->tail = NULL;
	return d;
}

void DestroyDequeue(dequeue* deq)
{
	node_deq* item = deq->head;
	node_deq* nextItem = NULL;
	while (item != deq->tail)
	{
		nextItem = item->next;
		BEET_free(item);
		item = nextItem;
	}
	BEET_free(deq);
}

BEET_bool dequeue_is_empty(dequeue* d)
{
	return d->head == NULL;
}

void dequeue_push_front(dequeue* d, void* value)
{
	node_deq* n = BEET_malloc(sizeof(node_deq));
	n->data = value;
	n->next = d->head;
	n->prev = NULL;
	if (d->tail == NULL)
	{
		d->head = d->tail = n;
	}
	else
	{
		d->head->prev = n;
		d->head = n;
	}
}

void dequeue_push_back(dequeue* d, void* value)
{
	node_deq* n = BEET_malloc(sizeof(node_deq));
	n->data = value;
	n->prev = d->tail;
	n->next = NULL;
	if (d->head == NULL)
	{
		d->head = d->tail = n;
	}
	else
	{
		d->tail->next = n;
		d->tail = n;
	}
}

void dequeue_pop_front(dequeue* d)
{
	node_deq* n = d->head;
	if (d->head == d->tail)
		d->head = d->tail = NULL;
	else
		d->head = n->next;
	BEET_free(n);
}

void dequeue_pop_back(dequeue* d)
{
	node_deq* n = d->tail;
	if (d->head == d->tail)
		d->head = d->tail = NULL;
	else
		d->tail = n->prev;
	BEET_free(n);
}

void* dequeue_front(dequeue* d)
{
	return d->head ? d->head->data : NULL;
}

void* dequeue_back(dequeue* d)
{
	return d->tail ? d->tail->data : NULL;
}

node_deq * dequeue_head(dequeue * d)
{
	return d->head;
}
// -----------------------------------------------------------------------------------------