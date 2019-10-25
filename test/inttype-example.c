// SPDX-License-Identifier: GPL-2.0-only
/*
 * Sample fifo int type implementation
 *
 * Copyright (C) 2010 Stefani Seibold <stefani@seibold.net>
 */

#include "fifo.h"
#include <stdio.h>
/*
 * This module shows how to create a int type fifo.
 */

/* fifo size in elements (ints) */
#define FIFO_SIZE	32

/*
 * define DYNAMIC in this example for a dynamically allocated fifo.
 *
 * Otherwise the fifo storage will be a part of the fifo structure.
 */
#define DYNAMIC

#ifdef DYNAMIC
static DECLARE_FIFO_PTR(test, int);
#else
static DEFINE_FIFO(test, int, FIFO_SIZE);
#endif

static const int expected_result[FIFO_SIZE] = {
	 3,  4,  5,  6,  7,  8,  9,  0,
	 1, 20, 21, 22, 23, 24, 25, 26,
	27, 28, 29, 30, 31, 32, 33, 34,
	35, 36, 37, 38, 39, 40, 41, 42,
};

static int testfunc(void)
{
	int		buf[6];
	int		i, j;
	unsigned int	ret;

	printf(  "int fifo test start\n");
	ret = fifo_out(&test, buf, 2);
	/* put values into the fifo */
	for (i = 0; i != 10; i++)
		fifo_put(&test, i);

	/* show the number of used elements */
	printf(  "fifo len: %u\n", fifo_len(&test));

	/* get max of 2 elements from the fifo */
	ret = fifo_out(&test, buf, 2);
	printf(  "ret: %d\n", ret);
	/* and put it back to the end of the fifo */
	ret = fifo_in(&test, buf, ret);
	printf(  "ret: %d\n", ret);

	/* skip first element of the fifo */
	printf(  "skip 1st element\n");
	fifo_skip(&test);

	/* put values into the fifo until is full */
	for (i = 20; fifo_put(&test, i); i++)
		;

	printf(  "queue len: %u\n", fifo_len(&test));

	/* show the first value without removing from the fifo */
	if (fifo_peek(&test, &i))
		printf(  "%d\n", i);

	/* check the correctness of all values in the fifo */
	j = 0;
	while (fifo_get(&test, &i)) {
		printf(  "item = %d\n", i);
		if (i != expected_result[j++]) {
			printf(  "value mismatch: test failed\n");
			return -1;
		}
	}
	printf(  "test passed\n");
	return 0;
}


int main(int argc, char **argv)
{
#ifdef DYNAMIC
	int ret;
	printf("test dynamic fifo\n");
	ret = fifo_alloc(&test, 32);
	if (ret) {
		printf("error fifo_alloc\n");
		return ret;
	}
#endif
	if (testfunc() < 0) {
#ifdef DYNAMIC
		fifo_free(&test);
#endif
		return -1;
	}

	return 0;
}
