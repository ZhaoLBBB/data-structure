// SPDX-License-Identifier: GPL-2.0-only
/*
 * Sample dynamic sized record fifo implementation
 *
 * Copyright (C) 2010 Stefani Seibold <stefani@seibold.net>
 */

#include "fifo.h"
#include <stdio.h>
#include <string.h>
/*
 * This module shows how to create a variable sized record fifo.
 */

/* fifo size in elements (bytes) */
#define FIFO_SIZE	128


/*
 * define DYNAMIC in this example for a dynamically allocated fifo.
 *
 * Otherwise the fifo storage will be a part of the fifo structure.
 */
#if 1
#define DYNAMIC
#endif

/*
 * struct fifo_rec_ptr_1 and  STRUCT_FIFO_REC_1 can handle records of a
 * length between 0 and 255 bytes.
 *
 * struct fifo_rec_ptr_2 and  STRUCT_FIFO_REC_2 can handle records of a
 * length between 0 and 65535 bytes.
 */

#ifdef DYNAMIC
struct fifo_rec_ptr_1 test;

#else
typedef STRUCT_FIFO_REC_1(FIFO_SIZE) mytest;

static mytest test;
#endif

static const char *expected_result[] = {
	"a",
	"bb",
	"ccc",
	"dddd",
	"eeeee",
	"ffffff",
	"ggggggg",
	"hhhhhhhh",
	"iiiiiiiii",
	"jjjjjjjjjj",
};

static int testfunc(void)
{
	char		buf[100];
	unsigned int	i;
	unsigned int	ret;
	struct { unsigned char buf[6]; } hello = { "hello" };

	printf( "record fifo test start\n");

	fifo_in(&test, &hello, sizeof(hello));

	/* show the size of the next record in the fifo */
	printf( "fifo peek len: %u\n", fifo_peek_len(&test));

	/* put in variable length data */
	for (i = 0; i < 10; i++) {
		memset(buf, 'a' + i, i + 1);
		fifo_in(&test, buf, i + 1);
	}

	/* skip first element of the fifo */
	printf( "skip 1st element\n");
	fifo_skip(&test);

	printf( "fifo len: %u\n", fifo_len(&test));

	/* show the first record without removing from the fifo */
	ret = fifo_out_peek(&test, buf, sizeof(buf));
	if (ret)
		printf( "%.*s\n", ret, buf);

	/* check the correctness of all values in the fifo */
	i = 0;
	while (!fifo_is_empty(&test)) {
		ret = fifo_out(&test, buf, sizeof(buf));
		buf[ret] = '\0';
		printf( "item = %.*s\n", ret, buf);
		if (strcmp(buf, expected_result[i++])) {
			printf( "value mismatch: test failed\n");
			return -1;
		}
	}
	if (i != ARRAY_SIZE(expected_result)) {
		printf( "size mismatch: test failed\n");
		return -1;
	}
	printf( "test passed\n");

	return 0;
}

int main(int argc, char **argv)
{
#ifdef DYNAMIC
	int ret;
	printf("test dynamic fifo\n");
	ret = fifo_alloc(&test, FIFO_SIZE);
	if (ret) {
		printf( "error fifo_alloc\n");
		return ret;
	}
#else
	INIT_FIFO(test);
#endif
	if (testfunc() < 0) {
#ifdef DYNAMIC
		fifo_free(&test);
#endif
		return -1;
	}

	return 0;
}
