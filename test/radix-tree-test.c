#include "radix-tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <limits.h>

#define ELEM_COUNT	256
#define CHAR_TAG	0
static void simple_test(void)
{
	RADIX_TREE(test_tree);
	char *name, ch;
	void **slot = NULL;
	int count = 0;
	struct radix_tree_iter iter;
 	fprintf(stdout, " ------------------------- Starting the basic test...\n");
	ch = 0;
	for(int i = 0; i < ELEM_COUNT; i++){
		name = malloc(10);
		memset(name, ch, 9);
		name[9] = '\0';
		if(name){
			if(!radix_tree_insert(&test_tree, i, name)){
				if(ch >= 'a' && ch <= 'z')
					radix_tree_tag_set(&test_tree, i, CHAR_TAG);
				ch++;
				count++;
			}
		}
	}
 	fprintf(stdout, " --------------insert %d elem in tree ...:\n", count);
	
	count = 0;
	radix_tree_for_each_slot(slot, &test_tree, &iter, 0){
		count++;
		name = *slot;
 		fprintf(stdout, "%c", name[0]);
	}
 	fprintf(stdout, "\n---------elem with tagged are:\n");
	radix_tree_for_each_tagged(slot, &test_tree, &iter, 0, CHAR_TAG){
		name = *slot;
 		fprintf(stdout, "%s\n", name);
	}

    fflush(stdout); // force echo to the screen... 
	for(int i = 0; i < ELEM_COUNT; i++){
		name = radix_tree_delete(&test_tree, i);
		if(!name){
			free(name);
		}
	}
}

static const long kLargePrime = 1398269;
static const long kEvenLargerPrime = 3021377;
#define NUM_TAG10e5  	0
#define NUM_TAG10e6     1
int a[1000];
static long insert_numbers(struct radix_tree_root *root, long n, long d)
{
  long k, count;
  long residue;
  fprintf(stdout, "Generating all of the numbers between 0 and %ld (using some number theory). ", d - 1);
  fflush(stdout); // force echo to the screen... 
  count = 0;
  for (k = 0; k < d; k++) {
    residue = (long) (((long long)k * (long long) n) % d);
    if(!radix_tree_insert(root, residue, (void*)(residue / 4 * 4 + 4))){
  		fflush(stdout); // force echo to the screen... 
	    if((residue + 1) % 100000 == 0){
		    if(radix_tree_tag_set(root, residue, NUM_TAG10e5)){
  				fprintf(stdout, "index  %ld ware tagged %d. \n", residue, NUM_TAG10e5);
  				fflush(stdout); // force echo to the screen... 
			}
	    }

	    if((residue + 1) % 1000000 == 0){
		    if(radix_tree_tag_set(root, residue, NUM_TAG10e6)){
  				fprintf(stdout, "index  %ld ware tagged %d. \n", residue, NUM_TAG10e6);
  				fflush(stdout); // force echo to the screen... 
			}
	    }
		count++;
    }
  }
  fprintf(stdout, "insert %ld item to tree \n", count - 1);
  fflush(stdout);
  return count;
}
static void tree_destroy(struct radix_tree_root *root, long n, long d, long max)
{
  long k;
  long residue;
  long count = 0;
  fprintf(stdout, "Delete all of the numbers between 0 and %ld. \n", d - 1);
  fflush(stdout); // force echo to the screen... 

  for (k = 0; k < d; k++) {
    residue = (long) (((long long)k * (long long) n) % d);
    if(radix_tree_delete(root, residue))
         count++;
  }
  fprintf(stdout, "after delete  %ld item the tree is %s \n", count - 1, radix_tree_empty(root)?"empty":"non-empty");
  fflush(stdout);

}
static void find_tagged_num( struct radix_tree_root *root, unsigned int tag)
{
	void **slot;
	long count, *tmparray[100];			//beacuse we know tagged elems less than 100
	int i;
	struct radix_tree_iter iter;
	count = 0;
	radix_tree_for_each_tagged(slot, root, &iter, 0, tag){
		count++;
	}
    fprintf(stdout, "%ld items in tree are tagged as %ld\n", count, tag);
    fflush(stdout);
	
	count = radix_tree_gang_lookup_tag(root, (void **)tmparray, 0, 100, tag);
    fprintf(stdout, "success find %ld items\n", count);
    fflush(stdout);
}
static void challenging_test(void)
{
	RADIX_TREE(test_tree);
	long count = 0;
	count = insert_numbers(&test_tree, kLargePrime, kEvenLargerPrime);
	find_tagged_num(&test_tree, NUM_TAG10e5);
	find_tagged_num(&test_tree, NUM_TAG10e6);
	tree_destroy(&test_tree, kLargePrime, kEvenLargerPrime, count);
}
int main(int argc, char **argv)
{
	simple_test();
    challenging_test();
	return 0;
}
