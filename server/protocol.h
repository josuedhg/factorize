#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

struct factorize_request {
	uint16_t number;
} __attribute__((packed));

struct factorize_response_header {
#define FACTORIZE_RESPONSE_TYPE_SUCCESS 0
#define FACTORIZE_RESPONSE_TYPE_ERROR 1
	uint16_t type:1;
	uint16_t reserved:15;
	uint16_t size;
} __attribute__((packed));

#define FACTORIZE_RESPONSE_ERROR_NO_ERROR 0
#define FACTORIZE_RESPONSE_ERROR_NOEMEM -1

#define FACTORIZE_RESPONSE_HEADER_SIZE sizeof(struct factorize_response_header)
#define FACTORIZE_RESPONSE_ITEM_SIZE sizeof(uint16_t)
#define FACTORIZE_RESPONSE_CALCULATE_SIZE(length) (FACTORIZE_RESPONSE_HEADER_SIZE + \
						  (FACTORIZE_RESPONSE_ITEM_SIZE * (length)))

#endif
