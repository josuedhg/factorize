#include <stdlib.h>
#include <math.h>

#include "factorize.h"

uint16_t factors(uint16_t input, uint16_t **factors)
{
	const uint8_t max_factors = UINT8_MAX;
	uint8_t factor_counter = 0;
	uint16_t *f = calloc(1, sizeof(uint16_t) * max_factors);
	if (f == NULL) return -1;
	for (int i=1;i<=input;i++)
		if (input%i==0)
			f[factor_counter++] = i;
	uint16_t *tmp = realloc(f, sizeof(uint16_t) * factor_counter);
	if (tmp != NULL) f = tmp;
	*factors = f;
	return factor_counter;
}
