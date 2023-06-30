#include <endian.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>

#include "network_connection.h"
#include "protocol.h"

void network_connection_init(struct network_connection *conn, struct result_queue *queue, char *host, char *port)
{
	conn->rqueue = queue;
	conn->host = host;
	conn->port = port;
}

int network_connection_request_report(struct network_connection *conn, uint16_t number)
{
	struct factorize_request request = { 0 };
	struct factorize_response_header response = { 0 };
	struct result_item *item = NULL;
	uint16_t *responses = NULL;
	struct addrinfo hints,*server;
	int r = 0;
	int socket_fd = 0;

	memset( &hints, 0, sizeof(hints) );
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo( conn->host, conn->port, &hints, &server ) != 0)
		return -1;

	if ((socket_fd = socket(server->ai_family,
			   server->ai_socktype,
			   server->ai_protocol)) == -1)
		goto FREE_ADDRESSINFO;

	if (connect(socket_fd,server->ai_addr,server->ai_addrlen) == -1)
		goto CLOSE_SOCKET;

	request.number = htole16(number);
	
	send(socket_fd, &request, sizeof(struct factorize_request), 0);

	if (recv(socket_fd, &response, sizeof(struct factorize_response_header), 0) < 1)
		goto CLOSE_SOCKET;

	*(uint32_t*)&response = le32toh(*(uint32_t *)&response);
	responses = calloc(1, FACTORIZE_RESPONSE_ITEM_SIZE * response.size);
	if (recv(socket_fd, responses, response.size * FACTORIZE_RESPONSE_ITEM_SIZE, 0) < 1)
		goto FREE_RESPONSES;

	for (int i = 0; i < response.size; i++)
		responses[i] = le16toh(responses[i]);

	item = calloc(1, sizeof(struct result_item));
	item->num = number;
	item->type = response.type;
	item->data = responses;
	item->size = response.size;
	result_queue_enqueue(conn->rqueue, item);
	
	close(socket_fd);
	freeaddrinfo(server);
	return 0;

FREE_RESPONSES:
	free(responses);
CLOSE_SOCKET:
	close(socket_fd);
FREE_ADDRESSINFO:
	freeaddrinfo(server);
	return -1;
}
