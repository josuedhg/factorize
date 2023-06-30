#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <endian.h>

#include <stdio.h>

#include "network.h"
#include "protocol.h"

int network_listener_init(struct network_listener *listener, int numconn, char *port, struct workqueue *queue)
{
	listener->server_socketfd = 0;
	listener->in = queue;
	listener->numconn = numconn;
	FD_ZERO(&listener->fd_set);
	struct addrinfo hints;
	struct addrinfo *serverinfo;
	struct sockaddr address;
	int r;

	memset( &hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo( 0, port, &hints, &serverinfo) != 0)
		goto FREE_ADDRESS;

	if ((listener->server_socketfd = socket(
			serverinfo->ai_family,
			serverinfo->ai_socktype,
			serverinfo->ai_protocol)) == -1)
		goto FREE_SOCKET;

	if (bind(listener->server_socketfd,
		 serverinfo->ai_addr,
		 serverinfo->ai_addrlen) == -1)
		goto FREE_SOCKET;

	if (listen(listener->server_socketfd, numconn) == -1)
		goto FREE_SOCKET;

	FD_SET(listener->server_socketfd, &listener->fd_set);
	freeaddrinfo(serverinfo);

	return 0;
FREE_SOCKET:
	close(listener->server_socketfd);
FREE_ADDRESS:
	freeaddrinfo(serverinfo);
	return -1;
}

int network_listener_execute(struct network_listener *listener)
{
	fd_set read_fd;
	socklen_t address_len = sizeof(struct sockaddr);
	struct sockaddr address;
	struct timeval tv;
	int fd = 0;

	tv.tv_sec = 0;
	tv.tv_usec = 1;

	read_fd = listener->fd_set;
	if (select(listener->numconn+1, &read_fd, NULL, NULL, &tv) == -1)
		return -1;
	
	for( fd=1; fd <= listener->numconn; fd++) {
		if( FD_ISSET(fd,&read_fd)) {
			if( fd==listener->server_socketfd ) {
				int clientfd = accept(
						listener->server_socketfd,
						(struct sockaddr *)&address,
						&address_len
						);
				if ( clientfd==-1 ) {
					return -1;
				}

				FD_SET(clientfd, &listener->fd_set);
			} else {
				struct factorize_request request;
				struct network_input_item *item;
				if (recv(fd, &request, sizeof(struct factorize_request), MSG_DONTWAIT) < 1) {
					FD_CLR(fd, &listener->fd_set);
					close(fd);
					continue;
				}

				item = calloc(1, sizeof(struct network_input_item));
				item->data = le16toh(request.number);
				item->client_socketfd = fd;
				workqueue_enqueue(listener->in, &item->item);
			}
		}
	}
	return 0;
}

void network_listener_exit(struct network_listener *listener)
{
	close(listener->server_socketfd);
}

void network_responser_init(struct network_responser *responser, struct workqueue *queue)
{
	responser->out = queue;
}

int network_responser_execute(struct network_responser *responser)
{
	struct workitem *item = workqueue_dequeue(responser->out);
	struct network_output_item *response = workitem_to_nework_output_item(item);
	struct factorize_response_header header = { 0 };
	uint8_t *buffer = NULL;
	uint8_t *buffer_filler = NULL;
	size_t buffer_size = 0;

	if (item == NULL)
		return 0;

	header.type = response->error? FACTORIZE_RESPONSE_TYPE_ERROR: FACTORIZE_RESPONSE_TYPE_SUCCESS;
	buffer_size = FACTORIZE_RESPONSE_CALCULATE_SIZE(response->size);
	header.size = response->size;

	buffer = calloc(1, buffer_size);
	buffer_filler = buffer;

	memcpy(buffer, &header, FACTORIZE_RESPONSE_HEADER_SIZE);
	*(uint32_t *)buffer_filler = htole32(*((uint32_t *)&header));
	buffer_filler += FACTORIZE_RESPONSE_HEADER_SIZE;
	
	printf(response->error? "ERROR: ": "RESPONSE: [");

	for (int i = 0; i < response->size; i++) {
		*((uint16_t *)buffer_filler) = htole16(response->data[i]);
		if (i != 0) printf(",");
		printf("%d", *(uint16_t *)buffer_filler);
		buffer_filler += FACTORIZE_RESPONSE_ITEM_SIZE;
	}
	printf(!response->error? "]\n": "\n");

	send(response->client_socketfd,
	     buffer,
	     buffer_size, 0);

	free(buffer);
	free(response->data);
	free(response);

	return 0;
}
