#include "set.h"
#include <stdlib.h>
#include <string.h>

#define MAX_NODE 100
#define INTERSECT_BEGIN 40
struct set_test{
	int val;
	struct set_node node;
};

static void destroy_func(struct set_node *elem)
{
	struct set_test *node;
	node = set_entry(elem, struct set_test, node);
	if(!node)
		free(node);
}

static struct set_node *copy_func(struct set_node *elem)
{
	struct set_test *node;
	struct set_test *new_node = NULL;
	node = set_entry(elem, struct set_test, node);
	new_node = (struct set_test*)malloc(sizeof(struct set_test));
	if(new_node)
		memcpy(new_node, node, sizeof(struct set_test));
	return &new_node->node;

}

static int cmp_int(struct set_node *elem1, struct set_node *elem2)
{
	int val1 = set_entry(elem1, struct set_test, node)->val;
	int val2 = set_entry(elem2, struct set_test, node)->val;
	return val1 - val2;
}


static struct set myset1;
static struct set myset2;
static struct set myset3;
int main(int argc, char **argv)
{
	unsigned int i, j;
	struct set_test *tmp_test_node;
	struct set_node *tmp_set_node;
	set_init(&myset1, cmp_int, copy_func, destroy_func);
	set_init(&myset2, cmp_int, copy_func, destroy_func);

	printf("Start test insert\n");
	for(i = 0; i < MAX_NODE; i++){
		tmp_test_node = (struct set_test *)malloc(sizeof(struct set_test));
		tmp_test_node->val = i;
		set_insert(&myset1, &tmp_test_node->node);

		tmp_test_node = (struct set_test *)malloc(sizeof(struct set_test));
		tmp_test_node->val = INTERSECT_BEGIN + i;
		set_insert(&myset2, &tmp_test_node->node);
	}

	printf("Start test intersection\n");
	set_intersection(&myset3, &myset2, &myset1);
	set_for_each(tmp_set_node, &myset3){
		tmp_test_node = set_entry(tmp_set_node, struct set_test, node);
		printf("%d, ", tmp_test_node->val);
	}
	printf("\n");
	printf("Start test destroy\n");
	set_for_each(tmp_set_node, &myset3){
		set_remove_destroy(&myset3, tmp_set_node);
	}
	printf("after delete set->num = %d\n", myset3.num);
	printf("\n");

	printf("\n");
	printf("Start test difference \n");
	set_difference(&myset3, &myset2, &myset1);
	set_for_each(tmp_set_node, &myset3){
		tmp_test_node = set_entry(tmp_set_node, struct set_test, node);
		printf("%d, ", tmp_test_node->val);
	}
	printf("\n");
	set_for_each(tmp_set_node, &myset3){
		set_remove_destroy(&myset3, tmp_set_node);
	}

	printf("Start test union \n");
	set_union(&myset3, &myset2, &myset1);
	set_for_each(tmp_set_node, &myset3){
		tmp_test_node = set_entry(tmp_set_node, struct set_test, node);
		printf("%d, ", tmp_test_node->val);
	}
	printf("\n");

	printf("Start test set_is_subset:\n");
	if(set_is_subset(&myset1, &myset3) && set_is_subset(&myset2, &myset3) && !set_is_subset(&myset1, &myset2))
		printf("OK\n");
	printf("Start test set_is_equal:\n");
	if(set_is_equal(&myset1, &myset1) && !set_is_equal(&myset2, &myset3))
		printf("OK\n");
	return 0;

}
