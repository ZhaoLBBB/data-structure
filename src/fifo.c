// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * A generic kernel FIFO implementation
 *
 * Copyright (C) 2009/2010 Stefani Seibold <stefani@seibold.net>
 */

#include "fifo.h"
#include <stdlib.h>
#include <string.h>
#include "log2.h"


/*
 * internal helper to calculate the unused elements in a fifo
 */
static inline unsigned int fifo_unused(struct __fifo *fifo)
{
	return (fifo->mask + 1) - (fifo->in - fifo->out);
}

int __fifo_alloc(struct __fifo *fifo, unsigned int size, size_t esize)
{
	/*
	 * round up to the next power of 2, since our 'let the indices
	 * wrap' technique works only in this case.
	 */
	size = roundup_pow_of_two(size);

	fifo->in = 0;
	fifo->out = 0;
	fifo->esize = esize;

	if (size < 2) {
		fifo->data = NULL;
		fifo->mask = 0;
		return -1;
	}

	fifo->data = malloc(esize * size);

	if (!fifo->data) {
		fifo->mask = 0;
		return -1;
	}
	fifo->mask = size - 1;

	return 0;
}

void __fifo_free(struct __fifo *fifo)
{
	free(fifo->data);
	fifo->in = 0;
	fifo->out = 0;
	fifo->esize = 0;
	fifo->data = NULL;
	fifo->mask = 0;
}

int __fifo_init(struct __fifo *fifo, void *buffer,
		unsigned int size, size_t esize)
{
	size /= esize;

	if (!is_power_of_2(size))
		size = rounddown_pow_of_two(size);

	fifo->in = 0;
	fifo->out = 0;
	fifo->esize = esize;
	fifo->data = buffer;

	if (size < 2) {
		fifo->mask = 0;
		return -1;
	}
	fifo->mask = size - 1;

	return 0;
}

static void fifo_copy_in(struct __fifo *fifo, const void *src,
		unsigned int len, unsigned int off)
{
	unsigned int size = fifo->mask + 1;
	unsigned int esize = fifo->esize;
	unsigned int l;

	off &= fifo->mask;
	if (esize != 1) {
		off *= esize;
		size *= esize;
		len *= esize;
	}
	l = len < (size- off) ? len : (size - off);

	memcpy(fifo->data + off, src, l);
	memcpy(fifo->data, src + l, len - l);
}

unsigned int __fifo_in(struct __fifo *fifo,
		const void *buf, unsigned int len)
{
	unsigned int l;

	l = fifo_unused(fifo);
	if (len > l)
		len = l;

	fifo_copy_in(fifo, buf, len, fifo->in);
	fifo->in += len;
	return len;
}

static void fifo_copy_out(struct __fifo *fifo, void *dst,
		unsigned int len, unsigned int off)
{
	unsigned int size = fifo->mask + 1;
	unsigned int esize = fifo->esize;
	unsigned int l;

	off &= fifo->mask;
	if (esize != 1) {
		off *= esize;
		size *= esize;
		len *= esize;
	}
	l = len < (size- off) ? len : (size - off);

	memcpy(dst, fifo->data + off, l);
	memcpy(dst + l, fifo->data, len - l);
}

unsigned int __fifo_out_peek(struct __fifo *fifo,
		void *buf, unsigned int len)
{
	unsigned int l;

	l = fifo->in - fifo->out;
	if (len > l)
		len = l;

	fifo_copy_out(fifo, buf, len, fifo->out);
	return len;
}

unsigned int __fifo_out(struct __fifo *fifo,
		void *buf, unsigned int len)
{
	len = __fifo_out_peek(fifo, buf, len);
	fifo->out += len;
	return len;
}


unsigned int __fifo_max_r(unsigned int len, size_t recsize)
{
	unsigned int max = (1 << (recsize << 3)) - 1;

	if (len > max)
		return max;
	return len;
}

#define	__KFIFO_PEEK(data, out, mask) \
	((data)[(out) & (mask)])
/*
 * __fifo_peek_n internal helper function for determinate the length of
 * the next record in the fifo
 */
static unsigned int __fifo_peek_n(struct __fifo *fifo, size_t recsize)
{
	unsigned int l;
	unsigned int mask = fifo->mask;
	unsigned char *data = fifo->data;

	l = __KFIFO_PEEK(data, fifo->out, mask);

	if (--recsize)
		l |= __KFIFO_PEEK(data, fifo->out + 1, mask) << 8;

	return l;
}

#define	__KFIFO_POKE(data, in, mask, val) \
	( \
	(data)[(in) & (mask)] = (unsigned char)(val) \
	)

/*
 * __fifo_poke_n internal helper function for storeing the length of
 * the record into the fifo
 */
static void __fifo_poke_n(struct __fifo *fifo, unsigned int n, size_t recsize)
{
	unsigned int mask = fifo->mask;
	unsigned char *data = fifo->data;

	__KFIFO_POKE(data, fifo->in, mask, n);

	if (recsize > 1)
		__KFIFO_POKE(data, fifo->in + 1, mask, n >> 8);
}

unsigned int __fifo_len_r(struct __fifo *fifo, size_t recsize)
{
	return __fifo_peek_n(fifo, recsize);
}

unsigned int __fifo_in_r(struct __fifo *fifo, const void *buf,
		unsigned int len, size_t recsize)
{
	if (len + recsize > fifo_unused(fifo))
		return 0;

	__fifo_poke_n(fifo, len, recsize);

	fifo_copy_in(fifo, buf, len, fifo->in + recsize);
	fifo->in += len + recsize;
	return len;
}

static unsigned int fifo_out_copy_r(struct __fifo *fifo,
	void *buf, unsigned int len, size_t recsize, unsigned int *n)
{
	*n = __fifo_peek_n(fifo, recsize);

	if (len > *n)
		len = *n;

	fifo_copy_out(fifo, buf, len, fifo->out + recsize);
	return len;
}

unsigned int __fifo_out_peek_r(struct __fifo *fifo, void *buf,
		unsigned int len, size_t recsize)
{
	unsigned int n;

	if (fifo->in == fifo->out)
		return 0;

	return fifo_out_copy_r(fifo, buf, len, recsize, &n);
}

unsigned int __fifo_out_r(struct __fifo *fifo, void *buf,
		unsigned int len, size_t recsize)
{
	unsigned int n;

	if (fifo->in == fifo->out)
		return 0;

	len = fifo_out_copy_r(fifo, buf, len, recsize, &n);
	fifo->out += n + recsize;
	return len;
}

void __fifo_skip_r(struct __fifo *fifo, size_t recsize)
{
	unsigned int n;

	n = __fifo_peek_n(fifo, recsize);
	fifo->out += n + recsize;
}

