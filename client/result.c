#include <stdio.h>
#include <stdlib.h>

#include "result.h"

void result_queue_init(struct result_queue *rqueue)
{
	rqueue->head = NULL;
	rqueue->tail = NULL;
	rqueue->counter = 0;
	pthread_mutex_init(&rqueue->lock, NULL);
	sem_init(&rqueue->sem, 0, 0);
}

void result_queue_enqueue(struct result_queue *rqueue, struct result_item *item)
{
	pthread_mutex_lock(&rqueue->lock);
	if (rqueue->counter == 0) {
		rqueue->head = item;
		rqueue->tail = item;
	} else {
		rqueue->tail->next = item;
		rqueue->tail = item;
	}
	rqueue->counter++;
	pthread_mutex_unlock(&rqueue->lock);
	sem_post(&rqueue->sem);
}

struct result_item *result_queue_dequeue(struct result_queue* rqueue)
{
	sem_wait(&rqueue->sem);
	pthread_mutex_lock(&rqueue->lock);
	struct result_item *item = rqueue->head;
	if (rqueue->counter > 0) {
		rqueue->head = rqueue->head->next;
		rqueue->counter--;
	}
	if (rqueue->head == NULL)
		rqueue->tail = NULL;
	pthread_mutex_unlock(&rqueue->lock);

	if (item != NULL)
		item->next = NULL;
	return item;
}

void result_queue_exit(struct result_queue *rqueue)
{
	pthread_mutex_destroy(&rqueue->lock);
	sem_destroy(&rqueue->sem);
}

void result_reporter_init(struct result_reporter *rep, struct result_queue *rqueue, log_t log, int nums)
{
	rep->rqueue = rqueue;
	rep->nums = nums;
	rep->log = log;
}

static void reporting(struct result_item *item, log_t log)
{
	log("%d: %s", item->num, !item->type?"[": "");
	for (int i = 0; i < item->size; i++) {
		if (i != 0) log(", ");
		log(!item->type?"%d": "ERROR: %d", item->data[i]);
	}
	log(!item->type?"]\n":"");
	free(item->data);
	free(item);
}

void result_reporter_loop(struct result_reporter *rep)
{
	for (int i = 0; i < rep->nums; i++) {
		struct result_item *item = result_queue_dequeue(rep->rqueue);
		reporting(item, rep->log);
	}
}
