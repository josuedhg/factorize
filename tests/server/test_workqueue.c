#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include "workqueue.h"

void test_workqueue_init(void **state)
{
	struct workqueue wq;
	workqueue_init(&wq);

	assert_null(wq.head);
	assert_null(wq.tail);
	assert_int_equal(wq.counter, 0);
	workqueue_exit(&wq);
}

void test_workqueue_enqueue(void **state)
{
	struct workitem wi = { 0 };
	struct workqueue wq;
	workqueue_init(&wq);
	workqueue_enqueue(&wq, &wi);
	assert_ptr_equal(wq.head, &wi);
	workqueue_exit(&wq);
}

void test_worqueue_dequeue(void **state)
{
	struct workqueue wq;
	struct workitem wi = { 0 };
	struct workitem *dequeue_wi = NULL;
	workqueue_init(&wq);
	workqueue_enqueue(&wq, &wi);

	dequeue_wi = workqueue_dequeue(&wq);
	assert_ptr_equal(&wi, dequeue_wi);
	workqueue_exit(&wq);
}

void test_workqueue_dequeue_multi(void **state)
{
	struct workqueue wq;
	struct workitem wi1 = { 0 };
	struct workitem wi2 = { 0 };
	struct workitem *dequeue_wi = NULL;

	workqueue_init(&wq);
	workqueue_enqueue(&wq, &wi1);
	workqueue_enqueue(&wq, &wi2);

	dequeue_wi = workqueue_dequeue(&wq);
	assert_ptr_equal(&wi1, dequeue_wi);
	
	dequeue_wi = workqueue_dequeue(&wq);
	assert_ptr_equal(&wi2, dequeue_wi);
	workqueue_exit(&wq);
}

int main()
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_workqueue_init),
		cmocka_unit_test(test_workqueue_enqueue),
		cmocka_unit_test(test_worqueue_dequeue),
		cmocka_unit_test(test_workqueue_dequeue_multi),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
