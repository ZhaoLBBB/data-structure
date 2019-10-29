#ifndef TYPES_H_
#define TYPES_H_

#ifndef container_of
#define container_of(ptr, type, member) \
	((type*)((char*)ptr - (char*)&(((type*)0)->member)))
#endif

#ifndef NULL
#define NULL (void*)0
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)   (sizeof(x)/sizeof(x[0]))
#endif

#define CONFIG_64BIT	y
#ifdef CONFIG_64BIT
#define BITS_PER_LONG    (64)
#else
#define BITS_PER_LONG    (32)  
#endif

#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))    
#define BITS_TO_LONGS(nr)   DIV_ROUND_UP(nr, 8/*CHAR_BIT*/ * BITS_PER_LONG)

typedef unsigned int bool;
#define true 1
#define false 0


#define BUG() while(1)

#define WRITE_ONCE(var, val) \
    (*((volatile typeof(val) *)(&(var))) = (val))

#define READ_ONCE(var) (*((volatile typeof(var) *)(&(var))))

static inline void * ERR_PTR(long error_)
{
    return (void *) error_;
}


#endif
