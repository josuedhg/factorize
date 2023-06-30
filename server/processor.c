#include <stdlib.h>

#include "factorize.h"
#include "network.h"
#include "processor.h"
#include "protocol.h"

void processor_init(struct processor *proc, struct workqueue *in, struct workqueue *out)
{
	proc->input = in;
	proc->output = out;
}

int processor_execute(struct processor *proc)
{
	struct workitem *item = workqueue_dequeue(proc->input);
	uint16_t *factor_list = NULL;
	int16_t factors_size = 0;
	struct network_input_item *in_item = workitem_to_nework_input_item(item);
	struct network_output_item *out_item = NULL;
	if (item == NULL)
		return 0;

	out_item = calloc(1, sizeof(struct network_output_item));	
	factors_size = factors(in_item->data, &factor_list);
	if (factors_size < 0) {
		out_item->error = factors_size;
		out_item->data = calloc(1, sizeof(uint16_t));
		*out_item->data = FACTORIZE_RESPONSE_ERROR_NOEMEM;
		out_item->size = 1;
	} else {
		out_item->error = 0;
		out_item->data = factor_list;
		out_item->size = factors_size;
	}
	out_item->client_socketfd = in_item->client_socketfd;
	workqueue_enqueue(proc->output, &out_item->item);

	free(in_item);
	return 0;
}
