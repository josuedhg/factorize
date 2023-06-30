#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "client.h"
#include "result.h"
#include "network_connection.h"

struct thread_context {
	pthread_t thread;
	uint16_t number;
	struct network_connection *conn;
};

void logger(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vfprintf(stdout, fmt, args);
	va_end(args);
}

void *request_receive(void *param)
{
	struct thread_context *context = (struct thread_context *)param;
	network_connection_request_report(context->conn, context->number);
	return NULL;
}

void *report_waiter(void *param)
{
	struct result_reporter *reporter = (struct result_reporter *)param;
	result_reporter_loop(reporter);
	return NULL;
}

int client_run(char *host, char *port, uint16_t *nums, int16_t nums_size)
{
	struct network_connection conn;
	struct result_reporter reporter;
	struct result_queue rqueue;
	pthread_t report_thread;
	struct thread_context *contexts = NULL;

	result_queue_init(&rqueue);
	network_connection_init(&conn, &rqueue, host, port);
	result_reporter_init(&reporter, &rqueue, logger, nums_size);

	pthread_create(&report_thread, NULL, report_waiter, &reporter);

	contexts = calloc(1, sizeof(struct thread_context) * nums_size);
	for (int i = 0; i < nums_size; i++) {
		contexts[i].conn = &conn;
		contexts[i].number = nums[i];
		pthread_create(&contexts[i].thread, NULL, request_receive, &contexts[i]);
	}

	for (int i = 0; i < nums_size; i++) {
		pthread_join(contexts[i].thread, NULL);
	}

	pthread_join(report_thread, NULL);
	free(contexts);
	return 0;
}
