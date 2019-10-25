#ifndef _FIBONACCI_HEAP_H_
#define _FIBONACCI_HEAP_H_
#if 0
enum {NIL = -1};
/**
 * Represents an edge in the graph, with its tip located at distance key.
 */
typedef struct fibonacci_heap_element {
  struct fibonacci_heap_element *parent;
  struct fibonacci_heap_element *child;
  struct fibonacci_heap_element *left;
  struct fibonacci_heap_element *right;
  int degree;
  float key;
  /* Decoration */
  int from;
  int to;
} fibonacci_heap_element_t;

typedef struct fibonacci_heap {
  int nb_nodes;
  fibonacci_heap_element_t *min;
  fibonacci_heap_element_t *root;
} fibonacci_heap_t;

fibonacci_heap_t *fibonacciHeapCreate();
void fibonacciHeapFree(fibonacci_heap_t *fh);
int fibonacciHeapIsEmpty(fibonacci_heap_t const *fh);
fibonacci_heap_element_t *fibonacciHeapMin(fibonacci_heap_t const *fh);
fibonacci_heap_element_t *fibonacciHeapNewElement(float key, int from, int to);
void fibonacciHeapAddRoot(fibonacci_heap_t *fh, fibonacci_heap_element_t *node, int count);
void fibonacciHeapDeleteRoot(fibonacci_heap_t *fh, fibonacci_heap_element_t *node, int count);
void fibonacciHeapInsert(fibonacci_heap_t *fh, fibonacci_heap_element_t *e);
fibonacci_heap_element_t *fibonacciHeapExtractMin(fibonacci_heap_t *fh);
#endif
#endif  // _FIBONACCI_HEAP_H_
