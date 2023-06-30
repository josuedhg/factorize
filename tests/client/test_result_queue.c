#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include "result.h"

void test_result_queue_init(void **state)
{
	struct result_queue rq;
	result_queue_init(&rq);

	assert_null(rq.head);
	assert_null(rq.tail);
	assert_int_equal(rq.counter, 0);
	result_queue_exit(&rq);
}

void test_result_queue_enqueue(void **state)
{
	struct result_item ri = { 0 };
	struct result_queue rq;
	result_queue_init(&rq);
	result_queue_enqueue(&rq, &ri);
	assert_ptr_equal(rq.head, &ri);
	result_queue_exit(&rq);
}

void test_worqueue_dequeue(void **state)
{
	struct result_queue rq;
	struct result_item ri = { 0 };
	struct result_item *dequeue_ri = NULL;
	result_queue_init(&rq);
	result_queue_enqueue(&rq, &ri);

	dequeue_ri = result_queue_dequeue(&rq);
	assert_ptr_equal(&ri, dequeue_ri);
	result_queue_exit(&rq);
}

void test_result_queue_dequeue_multi(void **state)
{
	struct result_queue rq;
	struct result_item ri1 = { 0 };
	struct result_item ri2 = { 0 };
	struct result_item *dequeue_ri = NULL;

	result_queue_init(&rq);
	result_queue_enqueue(&rq, &ri1);
	result_queue_enqueue(&rq, &ri2);

	dequeue_ri = result_queue_dequeue(&rq);
	assert_ptr_equal(&ri1, dequeue_ri);
	
	dequeue_ri = result_queue_dequeue(&rq);
	assert_ptr_equal(&ri2, dequeue_ri);
	result_queue_exit(&rq);
}

int main()
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_result_queue_init),
		cmocka_unit_test(test_result_queue_enqueue),
		cmocka_unit_test(test_worqueue_dequeue),
		cmocka_unit_test(test_result_queue_dequeue_multi),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
