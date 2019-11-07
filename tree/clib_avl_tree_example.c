#include <stdio.h>
#include "clib_avl_tree.h"

int my_compare(avl_key_t value1, avl_key_t value2) {
    if (*(int*)value1 > *(int*)value2) {
        return 1;
    } else if (*(int*)value1 < *(int*)value2) {
        return -1;
    }

    return 0;
}

void my_print(avl_key_t key) {
    printf("%d\t", *((int*)key));
}

int main() {
    avl_tree_t* tree = avl_tree_new(my_compare);
    if (NULL == tree) {
        puts("初始化失败!");
    }
    unsigned int i;
    int values[] = {
        0,  1,  2,  3,  4,  5,  6 , 7,  8,  9,  7,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 19,
        20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
        30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 
        40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50
    };

    printf("要插入的值\n");
    for (i = 0; i < sizeof (values) / sizeof (int); ++ i) {
        printf("%d|%d\t", i, values[i]);
        avl_tree_insert(tree, (avl_key_t)&values[i], (avl_value_t)&values[i]);
    }

    printf("\n树的节点的数 %d\n", avl_tree_num(tree));
    printf("\n前序遍历:\n");
    preorder_print_tree(avl_tree_root_node(tree), my_print);
    puts("\n=====\n=====\n");

    int key = values[1];

    printf("要查询的值：%d", key);

    void* res = avl_tree_lookup_value(tree, (avl_key_t)&key);
    if (NULL == res) {
        printf("\nNOT FOUND!\n");
    } else {
        printf("\n查询到的值是：%d\n", *(int*)res);
    }

    printf("\n树的节点个数：%d\n", avl_tree_num(tree));

    avl_tree_remove_node(tree, avl_tree_lookup_node(tree, (avl_key_t)&key));

    printf("\n删除后的节点 %d\n", avl_tree_num(tree));

    printf("\n前序遍历:\n");
    preorder_print_tree(avl_tree_root_node(tree), my_print);

    printf("\n中序遍历:\n");
    midorder_print_tree(avl_tree_root_node(tree), my_print);

    printf("\n后序遍历:\n");
    postorder_print_tree(avl_tree_root_node(tree), my_print);

    puts("\n==删除节点==\n");
    key = values[2];
    avl_tree_remove_node(tree, avl_tree_lookup_node(tree, (avl_key_t)&key));
    printf("\n删除2后前序遍历:\n");
    preorder_print_tree(avl_tree_root_node(tree), my_print);

    key = values[3];
    avl_tree_remove_node(tree, avl_tree_lookup_node(tree, (avl_key_t)&key));
    printf("\n删除3后前序遍历:\n");
    preorder_print_tree(avl_tree_root_node(tree), my_print);

    key = values[4];
    avl_tree_remove_node(tree, avl_tree_lookup_node(tree, (avl_key_t)&key));
    printf("\n删除4后前序遍历:\n");
    preorder_print_tree(avl_tree_root_node(tree), my_print);

    key = values[6];
    avl_tree_remove_node(tree, avl_tree_lookup_node(tree, (avl_key_t)&key));
    printf("\n删除6后前序遍历:\n");
    preorder_print_tree(avl_tree_root_node(tree), my_print);

    key = values[7];
    avl_tree_remove_node(tree, avl_tree_lookup_node(tree, (avl_key_t)&key));
    printf("\n删除7后前序遍历:\n");
    preorder_print_tree(avl_tree_root_node(tree), my_print);

    key = values[0];
    avl_tree_remove_node(tree, avl_tree_lookup_node(tree, (avl_key_t)&key));
    printf("\n删除0后前序遍历:\n");
    preorder_print_tree(avl_tree_root_node(tree), my_print);

    key = values[5];
    avl_tree_remove_node(tree, avl_tree_lookup_node(tree, (avl_key_t)&key));
    printf("\n删除5后前序遍历:\n");
    preorder_print_tree(avl_tree_root_node(tree), my_print);

    for(i = 0; i < sizeof(values); ++i) {
        key = values[i];
        avl_tree_remove_node(tree, avl_tree_lookup_node(tree, (avl_key_t)&key));
    }
    printf("\n全部删除后:\n");
    preorder_print_tree(avl_tree_root_node(tree), my_print);

    avl_tree_free(tree);
    printf("\n\n");

    return 0;
}
