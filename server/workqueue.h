#ifndef WORKQUEUE_H
#define WORKQUEUE_H

#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>

struct workitem {
	struct workitem *next;
};

struct workqueue {
	struct workitem *head;
	struct workitem *tail;
	uint8_t counter;
	pthread_mutex_t lock;
	sem_t sem;
};

void workqueue_init(struct workqueue *);
void workqueue_enqueue(struct workqueue *, struct workitem *);
struct workitem *workqueue_dequeue(struct workqueue*);
void workqueue_exit(struct workqueue *);

#endif
