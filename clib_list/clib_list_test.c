#include "clib_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


sDjList* list_create(void);
void list_free(sDjList* list);
sDjList* list_addNodeHead(sDjList* list, void* value);
sDjList* list_addNodeTail(sDjList* list, void* value);
void* list_delNode(sDjList* list, sDjListNode* delNode);
sDjListIter* list_getIterator(sDjList* list, int direction);
void list_freeIterator(sDjListIter* iter);
sDjListNode* list_nextData(sDjListIter* iter);
sDjList* list_dupList(sDjList* origList);
sDjListNode* list_search_value(sDjList* list, void* value);
sDjListNode* list_index(sDjList* list, long index);
void list_rewind_head(sDjList* list, sDjListIter* iter);
void list_rewind_tail(sDjList* list, sDjListIter* iter);
void list_rotate(sDjList* list);

//  从前到后的遍历
void print_list(sDjList* list);
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
    puts("\n");

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

    return 0;
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
}
