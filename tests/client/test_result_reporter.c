#define _GNU_SOURCE
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <cmocka.h>

#include "result.h"

void test_logger(const char *fmt, ...)
{
	va_list args;
	char *report_string = NULL;

	va_start(args, fmt);
	vasprintf(&report_string, fmt, args);
	check_expected(report_string);

	free(report_string);
	va_end(args);
}

void test_result_reporter_init(void **state)
{
	struct result_queue rq;
	result_queue_init(&rq);

	struct result_reporter rp;
	result_reporter_init(&rp, &rq, test_logger, 1);

	assert_int_equal(rp.log, test_logger);
	assert_int_equal(rp.nums, 1);
	assert_int_equal(rp.rqueue, &rq);
}

void test_result_reporter_loop(void **state)
{
	struct result_queue rq;
	result_queue_init(&rq);
	const int data_size = 1;
	struct result_item *item = NULL;

	struct result_reporter rp;
	result_reporter_init(&rp, &rq, test_logger, data_size);

	item = calloc(1, sizeof(struct result_item));
	item->data = calloc(1, sizeof(uint16_t));
	*item->data = 3;
	item->num = 1;
	item->size = 1;

	result_queue_enqueue(&rq, item);

	expect_string(test_logger, report_string, "1: [");
	expect_string(test_logger, report_string, "3");
	expect_string(test_logger, report_string, "]\n");
	result_reporter_loop(&rp);
}

void test_result_reporter_loop_multi_item(void **state)
{
	struct result_queue rq;
	result_queue_init(&rq);
	const int data_size = 2;
	struct result_item *item1 = NULL;
	struct result_item *item2 = NULL;

	struct result_reporter rp;
	result_reporter_init(&rp, &rq, test_logger, data_size);

	item1 = calloc(1, sizeof(struct result_item));
	item1->data = calloc(1, sizeof(uint16_t));
	*item1->data = 3;
	item1->num = 1;
	item1->size = 1;
	result_queue_enqueue(&rq, item1);

	item2 = calloc(1, sizeof(struct result_item));
	item2->data = calloc(1, sizeof(uint16_t));
	*item2->data = 3;
	item2->num = 2;
	item2->size = 1;
	result_queue_enqueue(&rq, item2);


	expect_string(test_logger, report_string, "1: [");
	expect_string(test_logger, report_string, "3");
	expect_string(test_logger, report_string, "]\n");
	expect_string(test_logger, report_string, "2: [");
	expect_string(test_logger, report_string, "3");
	expect_string(test_logger, report_string, "]\n");
	result_reporter_loop(&rp);
}

void test_result_reporter_loop_multi_data(void **state)
{
	struct result_queue rq;
	result_queue_init(&rq);
	const int data_size = 1;
	struct result_item *item1 = NULL;

	struct result_reporter rp;
	result_reporter_init(&rp, &rq, test_logger, data_size);

	item1 = calloc(1, sizeof(struct result_item));
	item1->data = calloc(1, sizeof(uint16_t) * 2);
	item1->data[0] = 3;
	item1->data[1] = 4;
	item1->num = 1;
	item1->size = 2;
	result_queue_enqueue(&rq, item1);

	expect_string(test_logger, report_string, "1: [");
	expect_string(test_logger, report_string, "3");
	expect_string(test_logger, report_string, ", ");
	expect_string(test_logger, report_string, "4");
	expect_string(test_logger, report_string, "]\n");
	result_reporter_loop(&rp);
}

void test_result_reporter_loop_error(void **state)
{
	struct result_queue rq;
	result_queue_init(&rq);
	const int data_size = 1;
	struct result_item *item1 = NULL;

	struct result_reporter rp;
	result_reporter_init(&rp, &rq, test_logger, data_size);

	item1 = calloc(1, sizeof(struct result_item));
	item1->data = calloc(1, sizeof(uint16_t));
	*item1->data = 3;
	item1->num = 1;
	item1->size = 1;
	item1->type = -1;
	result_queue_enqueue(&rq, item1);

	expect_string(test_logger, report_string, "1: ");
	expect_string(test_logger, report_string, "ERROR: 3");
	expect_string(test_logger, report_string, "");
	result_reporter_loop(&rp);
}
int main()
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_result_reporter_init),
		cmocka_unit_test(test_result_reporter_loop),
		cmocka_unit_test(test_result_reporter_loop_multi_item),
		cmocka_unit_test(test_result_reporter_loop_multi_data),
		cmocka_unit_test(test_result_reporter_loop_error),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
