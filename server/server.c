#include <pthread.h>
#include <stdbool.h>

#include "server.h"

#include "workqueue.h"
#include "network.h"
#include "processor.h"

static void *processor_loop(void *param)
{
	struct processor *processor = (struct processor *)param;
	int r = 0;
	while ((r = processor_execute(processor)) == 0);
	return NULL;
}

static void *network_responser_loop(void *param)
{
	struct network_responser *responser = (struct network_responser *)param;
	int r = 0;
	while ((r = network_responser_execute(responser)) == 0);
	return NULL;
}

static int network_listener_loop(void *param)
{
	struct network_listener *listener = (struct network_listener *)param;
	int r = 0;
	while ((r = network_listener_execute(listener)) == 0);
	return r;
}

int server_run(char *port, int numconn)
{
	struct workqueue inqueue;
	struct workqueue outqueue;
	struct processor processor;
	struct network_listener listener;
	struct network_responser responser;
	int r = 0;

	pthread_t processor_thrd;
	pthread_t responser_thrd;

	workqueue_init(&inqueue);
	workqueue_init(&outqueue);

	if (network_listener_init(&listener, numconn, port, &inqueue) == -1)
		return -1;

	network_responser_init(&responser, &outqueue);
	processor_init(&processor, &inqueue, &outqueue);

	pthread_create(&processor_thrd, NULL, processor_loop, &processor);
	pthread_create(&responser_thrd, NULL, network_responser_loop, &responser);

	r = network_listener_loop(&listener);

	pthread_join(processor_thrd, NULL);
	pthread_join(responser_thrd, NULL);

	network_listener_exit(&listener);

	return -1;
}
