#ifndef LOG2_H
#define LOG2_h
#include "bitops.h"
/**
 * is_power_of_2() - check if a value is a power of two
 * @n: the value to check
 *
 * Determine whether some value is a power of two, where zero is
 * *not* considered a power of two.
 * Return: 1 if @n is a power of 2, otherwise false.
 */
static inline int is_power_of_2(unsigned long n)
{
    return (n != 0 && ((n & (n - 1)) == 0));
}

static inline unsigned int roundup_pow_of_two(unsigned int n)
{
    return 1UL << fls(n - 1); 
}

static inline unsigned long roundup_pow_of_two_long(unsigned long n)
{
    return 1UL << fls_long(n - 1); 
}

/*
 * round down to nearest power of two
 */
static inline unsigned int rounddown_pow_of_two(unsigned int n)
{
    return 1UL << (fls(n) - 1); 
}

static inline unsigned long rounddown_pow_of_two_long(unsigned long n)
{
    return 1UL << (fls_long(n) - 1); 
}
#endif
