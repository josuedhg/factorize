#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>


#include "factorize.h"

void test_factors(void **state)
{
	uint16_t *results = NULL;
	int16_t results_len = 0;
	results_len = factors(4, &results);
	assert_int_equal(results_len, 3);
	assert_int_equal(results[0], 1);
	assert_int_equal(results[1], 2);
	assert_int_equal(results[2], 4);
	free(results);
}

void test_factors_big_number(void **state)
{
	uint16_t *results = NULL;
	int16_t results_len = 0;
	results_len = factors(32768, &results);
	assert_int_equal(results_len, 16);
	assert_int_equal(results[0], 1);
	assert_int_equal(results[1], 2);
	assert_int_equal(results[2], 4);
	assert_int_equal(results[3], 8);
	assert_int_equal(results[4], 16);
	assert_int_equal(results[5], 32);
	assert_int_equal(results[6], 64);
	assert_int_equal(results[7], 128);
	assert_int_equal(results[8], 256);
	assert_int_equal(results[9], 512);
	assert_int_equal(results[10], 1024);
	assert_int_equal(results[11], 2048);
	assert_int_equal(results[12], 4096);
	assert_int_equal(results[13], 8192);
	assert_int_equal(results[14], 16384);
	assert_int_equal(results[15], 32768);
}

int main()
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_factors),
		cmocka_unit_test(test_factors_big_number),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
