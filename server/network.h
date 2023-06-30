#ifndef NETWORK_H
#define NETWORK_H

#include <stdint.h>
#include <sys/types.h>

#include "workqueue.h"

struct network_info {
	int client_socketfd;
};

struct network_output_item {
	struct workitem item;
	uint8_t error;
	uint16_t *data;
	uint8_t size;
	int client_socketfd;
};
#define workitem_to_nework_output_item(ptr) ((struct network_output_item *)ptr)

struct network_input_item {
	struct workitem item;
	uint16_t data;
	int client_socketfd;
};
#define workitem_to_nework_input_item(ptr) ((struct network_input_item *)ptr)

struct network_listener {
	int server_socketfd;
	int numconn;
	fd_set fd_set;
	struct workqueue *in;
};

struct network_responser {
	struct workqueue *out;
};

int network_listener_init(struct network_listener *, int, char *, struct workqueue *);
int network_listener_execute(struct network_listener *);
void network_listener_exit(struct network_listener *);

void network_responser_init(struct network_responser *, struct workqueue *);
int network_responser_execute(struct network_responser *);

#endif
