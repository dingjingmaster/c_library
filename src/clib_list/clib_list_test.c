#include "clib_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  从前到后的遍历
void print_list(sDjList* list);

//  list 释放函数
void list_node_free(sDjListNode* list);

int main(int argc, char* argv[])
{
    char* value1 = "a";
    char* value2 = "b";
    char* value3 = "c";
    char* value4 = "d";

    printf ("list testing...");

    //  创建一个新的字符串
    sDjList* newList = list_create ();
    if (NULL == newList)
    {
        printf ("新链表创建失败\n");
        return -1;
    }

    //  添加字符串到头部
    newList = list_addNodeHead (newList, value1);
    if(NULL == newList)
    {
        return -1;
    }

    //  添加字符串到尾部
    newList = list_addNodeTail (newList, value2);
    if(NULL == newList)
    {
        return -1;
    }


    //  打印现在列表的信息
    print_list(newList);

    //  添加字符串到头部
    newList = list_addNodeHead (newList, value3);
    if(NULL == newList)
    {
        return -1;
    }

    //  添加字符串到尾部
    newList = list_addNodeTail (newList, value4);
    if(NULL == newList)
    {
        return -1;
    }

    //  打印现在列表的信息
    print_list(newList);

    //  list 值查找
    puts("list search data test");
    sDjListNode* valueNode = list_search_value(newList, "b");
    printf ("find: %s\n", valueNode ->value);

    //  list 复制
    puts("list dup test");
    sDjList* dupList = list_dupList(newList);
    //  打印复制的信息
    print_list(dupList);

    //  list index test
    puts("list index test");
    sDjListNode* indexNode1 = list_index(newList, 2);
    printf ("find: %s\n", indexNode1 ->value);
    sDjListNode* indexNode2 = list_index(newList, -1);
    printf ("find: %s\n", indexNode2 ->value);

    //  list rotate test
    puts("list rotate test");
    print_list (newList);
    list_rotate(newList);
    print_list (newList);

    //  list 删除节点测试
    puts("list delete node");
    list_delNode(newList, indexNode1);
    print_list (newList);

    //  list free test
    puts("list free test");
    LIST_SET_FREE_METHOD (newList, NULL);
    list_free(newList);
    print_list (newList);
    printf ("list num = %d\n", newList ->len);


    return 0;
}

void list_node_free(sDjListNode* listNode)
{
    if(NULL == listNode)
    {
        return;
    }

    free(listNode);
}

void print_list(sDjList* list)
{
    if(NULL == list)
    {
        return;
    }

    sDjListIter* iter = NULL;
    sDjListNode* node = NULL;

    //  获取 list 的迭代器
    iter = list_getIterator (list, ITERATOR_START_HEAD);
    if(NULL == iter)
    {
        puts("迭代器出错");
        return;
    }

    puts("\n");
    //  遍历并输出
    while(NULL != (node = list_nextData (iter)))
    {
        printf ("%s\t", node ->value);
    }
    puts("\n");

    list_freeIterator(iter);
}
