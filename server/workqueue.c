#include <stdlib.h>

#include "workqueue.h"

void workqueue_init(struct workqueue *queue)
{
	queue->head = NULL;
	queue->tail = NULL;
	queue->counter = 0;
	pthread_mutex_init(&queue->lock, NULL);
	sem_init(&queue->sem, 0, 0);
}

void workqueue_enqueue(struct workqueue *queue, struct workitem *item)
{
	pthread_mutex_lock(&queue->lock);
	if (queue->counter == 0) {
		queue->head = item;
		queue->tail = item;
	} else {
		queue->tail->next = item;
		queue->tail = item;
	}
	queue->counter++;
	pthread_mutex_unlock(&queue->lock);
	sem_post(&queue->sem);
}

struct workitem *workqueue_dequeue(struct workqueue* queue)
{
	sem_wait(&queue->sem);
	pthread_mutex_lock(&queue->lock);
	struct workitem *item = queue->head;
	if (queue->counter > 0) {
		queue->head = queue->head->next;
		queue->counter--;
	}
	if (queue->head == NULL)
		queue->tail = NULL;
	pthread_mutex_unlock(&queue->lock);

	if (item != NULL)
		item->next = NULL;
	return item;
}

void workqueue_exit(struct workqueue *queue)
{
	pthread_mutex_destroy(&queue->lock);
	sem_destroy(&queue->sem);
}
