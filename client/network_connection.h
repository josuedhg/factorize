#ifndef NETWORK_H
#define NETWORK_H

#include <stdint.h>

#include "result.h"

struct network_connection {
	char *host;
	char *port;
	struct result_queue *rqueue;
};

void network_connection_init(struct network_connection *, struct result_queue *, char *, char *);
int network_connection_request_report(struct network_connection *, uint16_t);

#endif
