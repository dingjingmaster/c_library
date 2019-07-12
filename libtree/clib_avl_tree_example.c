#include <stdio.h>
#include "clib_avl_tree.h"

int my_compare(void* value1, void* value2) {

    if (*(int*)value1 > *(int*)value2) {
        return 1;
    } else if (*(int*)value1 < *(int*)value2) {
        return -1;
    }

    return 0;
}

void my_print(void* key) {
    printf("%d\t", *((int*)key));
}

int main() {
    avl_tree_t* tree = avl_tree_new(my_compare);
    unsigned int i;
    int values[] = { 0, 1, 2, 3, 4, 5, 6 };

    for (i = 0; i < sizeof (values) / sizeof (int); ++ i) {
        printf("%d|%d\t", i, values[i]);
        avl_tree_insert(tree, &values[i], &values[i]);
    }
    puts("\n");

    int key = values[1];

    void* res = avl_tree_lookup_value(tree, &key);
    if (NULL == res) {
        printf("\nNOT FOUND!\n");
    } else {
        printf("\nlookup value is %d\n", *(int*)res);
    }

    printf("\ntree's node number is %d\n", avl_tree_num(tree));

    avl_tree_remove_node(tree, res);

    printf("\nafter delete option tree's node number is %d\n", avl_tree_num(tree));

    printf("\nbefore order traversal binary tree. result as follow:\n");
    preorder_print_tree(avl_tree_root_node(tree), my_print);

    printf("\nMiddle order traversal binary tree. result as follow:\n");
    midorder_print_tree(avl_tree_root_node(tree), my_print);

    printf("\npost order traversal binary tree. result as follow:\n");
    postorder_print_tree(avl_tree_root_node(tree), my_print);

    avl_tree_free(tree);
    printf("\n\n");

    return 0;
}
