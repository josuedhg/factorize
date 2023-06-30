#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>

#include "network.h"
#include "processor.h"

void test_processor_init(void **status)
{
	struct workqueue iwq;
	struct workqueue owq;
	struct processor p;
	processor_init(&p, &iwq, &owq);

	assert_ptr_equal(p.input, &iwq);
	assert_ptr_equal(p.output, &owq);
}

void test_proccessor_exec(void **status)
{
	struct workqueue iwq;
	struct workqueue owq;
	struct processor p;
	processor_init(&p, &iwq, &owq);
	workqueue_init(&iwq);
	workqueue_init(&owq);
	const int fake_fd = 1;

	// fill input queue
	struct network_input_item *ni = calloc(1, sizeof(struct network_input_item));
	ni->client_socketfd = fake_fd;
	ni->data = 4;

	workqueue_enqueue(&iwq, &ni->item);

	// executing
	processor_execute(&p);

	// getting results
	struct workitem *i = workqueue_dequeue(&owq);
	assert_non_null(i);

	struct network_output_item *no = workitem_to_nework_output_item(i);

	// testing
	assert_int_equal(no->client_socketfd, fake_fd);
	assert_int_equal(no->size, 3);
	assert_int_equal(no->data[0], 1);
	assert_int_equal(no->data[1], 2);
	assert_int_equal(no->data[2], 4);

	free(no->data);
	free(no);
}

int main()
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_processor_init),
		cmocka_unit_test(test_proccessor_exec),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
