/*************************************************************************
> FileName: clib-double-list-example.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年11月07日 星期四 21时26分14秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "../../core/c/list/clib-double-list.h"

void print_list(double_list_t* list);
void list_node_free(double_list_node_t* list);

int main(int argc, char* argv[]) {
    char* value1 = "a";
    char* value2 = "b";
    char* value3 = "c";
    char* value4 = "d";

    printf ("list testing...");

    double_list_t* newList = list_create ();
    if (NULL == newList) {
        printf ("新链表创建失败\n");
        return -1;
    }

    newList = list_add_node_head (newList, value1);
    if(NULL == newList) {
        return -1;
    }

    newList = list_add_node_tail (newList, value2);
    if(NULL == newList) {
        return -1;
    }


    print_list(newList);

    newList = list_add_node_head (newList, value3);
    if(NULL == newList) {
        return -1;
    }

    newList = list_add_node_tail (newList, value4);
    if(NULL == newList) {
        return -1;
    }

    print_list(newList);

    puts("list search data test");
    double_list_node_t* valueNode = list_search_value(newList, "b");
    printf ("find: %s\n", (char*)(valueNode ->value));

    puts("list dup test");
    double_list_t* dupList = list_dup_list(newList);
    //  打印复制的信息
    print_list(dupList);

    //  list index test
    puts("list index test");
    double_list_node_t* indexNode1 = list_index(newList, 2);
    printf ("find: %s\n", (char*)(indexNode1 ->value));
    double_list_node_t* indexNode2 = list_index(newList, -1);
    printf ("find: %s\n", (char*)(indexNode2 ->value));

    //  list rotate test
    puts("list rotate test");
    print_list (newList);
    list_rotate(newList);
    print_list (newList);

    //  list 删除节点测试
    puts("list delete node");
    list_del_node(newList, indexNode1);
    print_list (newList);

    //  list free test
    puts("list free test");
    LIST_SET_FREE_METHOD (newList, NULL);
    list_free(newList);
    print_list (newList);
    printf ("list num = %lu\n", newList ->len);


    return 0;
}

void list_node_free(double_list_node_t* listNode) {
    if(NULL == listNode) {
        return;
    }

    free(listNode);
}

void print_list(double_list_t* list) {
    if(NULL == list) {
        return;
    }

    double_list_iter_t* iter = NULL;
    double_list_node_t* node = NULL;

    iter = list_get_iterator (list, ITERATOR_START_HEAD);
    if(NULL == iter)
    {
        puts("迭代器出错");
        return;
    }

    puts("\n");
    //  遍历并输出
    while(NULL != (node = list_next_data (iter)))
    {
        printf ("%s\t", (char*)(node ->value));
    }
    puts("\n");

    list_free_iterator(iter);
}

