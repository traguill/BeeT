#ifndef __BEET_DEQUEUE_H__
#define __BEET_DEQUEUE_H__

typedef struct BeeT_dequeue
{
	void* data;
	int size;
	int itemSize;
	int maxSize;

	void(*push_back)(dequeue*, void*);
	void(*push_front)(dequeue*, void*);
	void(*pop_back)(dequeue*);
	void(*pop_front)(dequeue*);
	void*(*back)(dequeue*);
	void*(*front)(dequeue*);

}dequeue;

dequeue* InitDequeue(int itemSize);
#endif // !__BEET_DEQUEUE_H__

