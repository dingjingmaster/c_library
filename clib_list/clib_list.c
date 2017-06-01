#include "clib_list.h"

//  链表创建
sDjList *list_create()
{
    sDjList*   list = NULL;
    if(list == (sDjList*)malloc(sizeof(sDjList)) == NULL)
    {
        return NULL;
    }

    //  初始化属性
    list ->head = NULL;
    list ->tail = NULL;
    list ->len = 0;
    list ->dup = NULL;
    list ->free = NULL;
    list ->match = NULL;

    return list;
}

void list_free(sDjList *list)
{
    unsigned long len = 0;
    sDjListNode* pCur = NULL;
    sDjListNode* pNext = NULL;

    //  指向头指针
    pCur = list ->head;

    //  遍历整个链表
    len = list ->len;

    while(--len)
    {
        //  保存下一个节点
        pNext = pCur ->next;

        //  调用值的释放函数释放
        if(list ->free)
            list ->free(pCur ->value);

        //  释放节点结构
        free(pCur);

        //  开始释放下一个节点
        pCur = pNext;
    }
}

sDjList *list_addNodeHead(sDjList *list, void *value)
{
    sDjListNode* node;

    //  分配内存
    if(node = (sDjListNode*)malloc(sizeof(sDjListNode)) == NULL)
        return NULL;

    //  保存值的指针
    node ->value = value;

    //  添加节点到链表头部
    if(0 == list ->len)
    {
        //  链表中无节点
        list ->head = node;
        list ->tail= node;
        node ->next = NULL;
        node ->pre = NULL;
    }
    else
    {
        //  连种种元素不为0
        node ->next = list ->head;
        list ->head ->pre = node;
        list ->head = node;
        node ->pre = NULL;
    }

    //  更新链表节点数
    list ->len ++;

    return list;
}

sDjList *list_addNodeTail(sDjList *list, void *value)
{
    //  创建新的节点
    sDjListNode* newNode;

    //  分配内存
    if(newNode = (sDjListNode*)malloc(sizeof(sDjListNode) == NULL))
        return NULL;

    //  赋值
    newNode ->value = value;

    //  如果链表为空
    if(0 == list ->len)
    {
        list ->head = newNode;
        list ->tail = newNode;
        newNode ->pre = NULL;
        newNode ->next = NULL;
    }
    else
    {
        //  如果链表中有元素的情况下
        list ->tail ->next = newNode;
        newNode ->pre = list ->tail;
        list ->tail = newNode;
        newNode ->next = NULL;
    }

    //  更新链表信息
    list ->len ++;

    return list;
}

void *list_delNode(sDjList *list, sDjListNode *delNode)
{
    //  调节前置节点的指针
    if(NULL != delNode ->pre)
    {
        delNode ->pre ->next = delNode ->next;
    }
    else
    {
        list ->head = delNode ->next;
    }

    //  调节后置节点的指针
    if(NULL != delNode ->next)
    {
        delNode ->next ->pre = delNode ->pre;
    }
    else
    {
        list ->tail = delNode ->pre;
    }

    //  释放值
    if(list ->free)
        list ->free(delNode ->value);

    //  释放节点
    free(delNode);

    //  链表节点数减一
    list ->len --;
}

sDjListIter *list_getIterator(sDjList *list, int direction)
{
    //  为迭代器分配内存
    sDjListIter* iter;

    if((iter = (sDjListIter*)malloc(sizeof(sDjListIter))) == NULL)
        return NULL;

    //  根据迭代器的方向选择起始位置
    if(direction == ITERATOR_START_HEAD)
        iter ->next = list ->head;
    else if(direction == ITERATOR_START_TAIL)
        iter ->next = list ->tail;
    else
        return NULL;

    //  记录迭代的方向
    iter ->direction = direction;

    return iter;
}

void list_freeIterator(sDjListIter *iter)
{
    if(NULL == iter)
        return;

    iter ->next = NULL;

    free(iter);
}

sDjListNode *list_nextData(sDjListIter *iter)
{
    sDjListNode* current = iter ->next;

    if(NULL != current)
    {
        //  根据方向选择下一个节点
        if(iter ->direction == ITERATOR_START_HEAD)
            //  保存下一个节点，防止当前节点被删除而造成指针丢失
            iter ->next = current ->next;
        else if(iter ->direction == ITERATOR_START_TAIL)
            iter ->next = current ->pre;
    }

    return current;
}
