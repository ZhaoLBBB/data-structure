#include "radix-tree.h"

#define ELEM_COUNT	256
#define CHAR_TAG	0
int main(int argc, char **argv)
{
	char *name, ch;
	void **slot = NULL;
	int count;
	struct radix_tree_iter iter;
	RADIX_TREE(test_tree);
	printf("========start test insert and iter================\n");
	ch = 0;
	for(int i = 0; i < ELEM_COUNT; i++){
		name = malloc(10);
		memset(name, ch, 9);
		name[9] = '\0';
		if(name){
			radix_tree_insert(&test_tree, i, name);
			if(ch >= 'a' && ch <= 'z')
				radix_tree_tag_set(&test_tree, i, CHAR_TAG);
			ch++;
		}
	}
	count = 0;
	radix_tree_for_each_slot(slot, &test_tree, &iter, 0){
		count++;
		name = *slot;
		printf("%c", name[0]);
	}
	printf("\n===========radix tree has %d element\n",count);
	
	printf("\n===========radix tree tagged element\n");
	radix_tree_for_each_tagged(slot, &test_tree, &iter, 0, CHAR_TAG){
		name = *slot;
		printf("%s\n", name);
	}
#ifdef DEBUG
	radix_tree_dump(&test_tree);	
#endif
	printf("\n===========start test deletion ========\n");
	for(int i = 0; i < ELEM_COUNT; i++){
		name = radix_tree_delete(&test_tree, i);
		printf("%c", name[0]);
		free(name);
	}
	printf("\n============after delete %d elem radix tree is empty? %s========\n", ELEM_COUNT, radix_tree_empty(&test_tree) ?"true":"false");
	return 0;
}
