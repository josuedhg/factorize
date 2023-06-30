#ifndef RESULT_H
#define RESULT_H

#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>

typedef void (*log_t)(const char *, ...);

struct result_item {
	uint8_t type;
	uint16_t num;
	uint16_t *data;
	uint8_t size;
	struct result_item *next;
};

struct result_queue {
	struct result_item *head;
	struct result_item *tail;
	uint8_t counter;
	pthread_mutex_t lock;
	sem_t sem;
};

struct result_reporter {
	struct result_queue *rqueue;
	int nums;
	log_t log;
};

void result_queue_init(struct result_queue *);
void result_queue_enqueue(struct result_queue *, struct result_item *);
struct result_item *result_queue_dequeue(struct result_queue *);
void result_queue_exit(struct result_queue *);

void result_reporter_init(struct result_reporter *, struct result_queue *, log_t, int);
void result_reporter_loop(struct result_reporter *);

#endif
