#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "fibonacci_heap.h"

/**
 * Create an empty Fibonacci heap.
 */
fibonacci_heap_t *fibonacciHeapCreate() {
  fibonacci_heap_t *fh =
    (fibonacci_heap_t *) malloc(sizeof(fibonacci_heap_t));
    if(!fh){
        fprintf(stderr, "Unable to allocate memory.\n");
        exit(-1);
    }
  fh->nb_nodes = 0;
  fh->min = NULL;
  fh->root = NULL;

  return fh;
}


/**
 * Free a Fibonacci heap.
 */
static void fibonacciHeapFreeSubTree(fibonacci_heap_element_t *root) {
  if (root == NULL) {
    return;
  }

  if (root->right != root) {  // Root is not an only child
    fibonacci_heap_element_t *current = root->right;
    fibonacci_heap_element_t *tmp;

    while (current != root) {
      tmp = current->right;
      fibonacciHeapFreeSubTree(current);
      current = tmp;
    }
  }
  fibonacciHeapFreeSubTree(root->child);
  free(root);
}
void fibonacciHeapFree(fibonacci_heap_t *fh) {
  fibonacciHeapFreeSubTree(fh->root);
  free(fh);
}


/**
 * Check whether a Fibonacci heap is empty or not.
 */
int fibonacciHeapIsEmpty(fibonacci_heap_t const *fh) {
  return fh->nb_nodes == 0;
}


/**
 * Find the minimum of the Fibonacci heap.
 */
fibonacci_heap_element_t *fibonacciHeapMin(fibonacci_heap_t const *fh) {
  return fh->min;
}


/**
 * Crate an empty new element ready to be inserted in a fibonacci heap.
 */
fibonacci_heap_element_t *fibonacciHeapNewElement(float key, int from, int to) {
  fibonacci_heap_element_t *e = malloc(sizeof(fibonacci_heap_element_t));
  if(!e){
      fprintf(stderr, "Unable to allocate memory.\n");
      exit(-1);
  }
  e->key = key;
  e->degree = 0;
  e->child = NULL;
  e->parent = NULL;
  e->left = e;
  e->right = e;
  /* Decorations */
  e->from = from;
  e->to = to;
  return e;
}


/**
 * Add a node to the roots of a Fibonacci heap.
 */
void fibonacciHeapAddRoot(fibonacci_heap_t *fh, fibonacci_heap_element_t *node, int count) {
  fibonacci_heap_element_t *root = fh->root;
  node->parent = NULL;

  fh->root = node;

  if (root != NULL) {  // Non-empty heap, update the lists
    node->right = root;
    node->left = root->left;
    root->left->right = node;
    root->left = node;
  }
  else {
    node->left = node;
    node->right = node;
  }

  fh->nb_nodes += count;
}


/**
 * Delete a node from the roots of a Fibonacci heap.
 *
 * User must manually free.
 */
void fibonacciHeapDeleteRoot(fibonacci_heap_t *fh, fibonacci_heap_element_t *node, int count) {
  if (node == node->right) {
    fh->root = NULL;
  }
  else {
    node->right->left = node->left;
    node->left->right = node->right;
    fh->root = node->right;
  }
  fh->nb_nodes -= count;
}


/**
 * Insert an element in a Fibonacci heap.
 */
void fibonacciHeapInsert(fibonacci_heap_t *fh, fibonacci_heap_element_t *e) {
  fibonacciHeapAddRoot(fh, e, 1);

  if (fh->min == NULL || e->key < fh->min->key) {
    fh->min = e;
  }
}


/**
 * Extract the minimum from a Fibonacci heap.
 */
static void fibonacciHeapAppend(fibonacci_heap_element_t *x, fibonacci_heap_element_t *y) {
  fibonacci_heap_element_t *child = x->child;
  y->parent = x;
  x->child = y;
  if (child != NULL) {  // Update lists
    y->right = child;
    y->left = child->left;
    child->left->right = y;
    child->left = y;
  }
  else {
    y->left = y;
    y->right = y;
  }
  x->degree++;
}
static void fibonacciHeapLinkHeaps(fibonacci_heap_t *fh, fibonacci_heap_element_t *y, fibonacci_heap_element_t *x) {
  fibonacciHeapDeleteRoot(fh, y, 0);
  fibonacciHeapAppend(x, y);
}
static void fibonacciHeapConsolidate(fibonacci_heap_t *fh) {
  const int max_deg = (int) floor(log(fh->nb_nodes) / log((1+sqrt(5)) / 2)) + 1;
  fibonacci_heap_element_t **A = (fibonacci_heap_element_t **) malloc(sizeof(fibonacci_heap_element_t *) * max_deg);
    if(!A){
        fprintf(stderr, "Unable to allocate memory.\n");
        exit(-1);
    }
  for (int i = 0; i < max_deg; i++) {
    A[i] = NULL;
  }

  fibonacci_heap_element_t *root_left = fh->root->left;  // Keep original element in memory
  fibonacci_heap_element_t *w = fh->root;

  int left_seen = 0;
  // Iterate over all the roots
  while (1) {
    if (w == root_left) {
      left_seen = 1;
    }
    fibonacci_heap_element_t *x = w;
    fibonacci_heap_element_t *next = w->right;
    int d = x->degree;
    while (A[d] != NULL) {
      fibonacci_heap_element_t *y = A[d];
      if (x->key > y->key) {
        fibonacci_heap_element_t *tmp = y;
        y = x;
        x = tmp;
      }
      fibonacciHeapLinkHeaps(fh, y, x);
      A[d] = NULL;
      d++;
    }
    A[d] = x;
    w = next;
    if (left_seen == 1) {
      break;
    }
  }

  fh->min = NULL;
  for (int i = 0; i < max_deg; i++) {
    if (A[i] != NULL) {
      /* fibonacciHeapAddRoot(fh, A[i], 0); */
      if (fh->min == NULL || A[i]->key < fh->min->key) {
        fh->min = A[i];
      }
    }
  }
  free(A);
}
fibonacci_heap_element_t *fibonacciHeapExtractMin(fibonacci_heap_t *fh) {
  fibonacci_heap_element_t *z = fh->min;
  if (z != NULL) {
    if (z->child != NULL) {  // Put all the childs of z at the root level
      fibonacci_heap_element_t *current = z->child;
      fibonacci_heap_element_t *tmp;

      do {
        tmp = current->right;
        fibonacciHeapAddRoot(fh, current, 0);
        current = tmp;
      } while (current != z->child);
    }

    fibonacciHeapDeleteRoot(fh, z, 1);

    if (z == z->right) {
      fh->min = NULL;
    }
    else {
      fh->min = z->right;
      fibonacciHeapConsolidate(fh);
    }
  }

  return z;
}
