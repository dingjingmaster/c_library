#include "clib_list.h"

//  链表创建
sDjList *list_create()
{
    sDjList*   list = NULL;
    if((list = (sDjList*)malloc(sizeof(sDjList))) == NULL)
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

sDjList *list_addNodeHead(sDjList* list, void* value)
{
    sDjListNode* node = NULL;

    //  分配内存
    if((node = (sDjListNode*)malloc(sizeof(sDjListNode))) == NULL)
    {
        return NULL;
    }

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
    sDjListNode* newNode = NULL;

    //  分配内存
    if((newNode = (sDjListNode*)malloc(sizeof(sDjListNode))) == NULL)
    {
        return NULL;
    }

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

sDjList *list_dupList(sDjList *orig)
{
    sDjList* copyList = NULL;
    sDjListIter* iter = NULL;
    sDjListNode* listNode = NULL;

    //  创建新的链表
    if((copyList = list_create ()) == NULL)
    {
        return NULL;
    }

    //  给新的链表设置处理函数
    copyList ->dup = orig ->dup;
    copyList ->free = orig ->free;
    copyList ->match = orig ->match;

    //  迭代链表并把值加入到新的链表中
    iter = list_getIterator (orig, ITERATOR_START_HEAD);
    while (NULL != (listNode = list_nextData (iter)))
    {
        void* value = NULL;
        //  复制节点并加入
        if(copyList ->dup)
        {
            value = copyList ->dup(listNode ->value);

            if(NULL == value)
            {
                list_free(copyList);
                list_freeIterator(iter);

                return NULL;
            }
        }
        else
        {
            value = listNode ->value;
        }

        //  将节点添加到链表
        if(NULL == list_addNodeTail (copyList, value))
        {
            list_free(value);
            list_freeIterator(iter);

            return NULL;
        }
    }

    //  释放迭代器
    list_freeIterator(iter);

    return copyList;
}

sDjListNode *list_search_value(sDjList *list, void *value)
{
    sDjListIter*    iter = NULL;
    sDjListNode*    listNode = NULL;

    //  迭代整个列表
    iter = list_getIterator (list, ITERATOR_START_HEAD);

    while(NULL != (listNode = list_nextData (iter)))
    {
        //  比较
        if(list ->match)
        {
            if(list ->match(listNode ->value, value))
            {
                //  找到了 释放迭代器， 返回节点
                list_freeIterator(iter);

                return listNode;
            }
        }
        else
        {
            if(value == listNode ->value)
            {
                //  找到
                list_freeIterator(iter);

                return listNode;
            }
        }
    }

    //  未找到
    list_freeIterator(iter);

    return NULL;
}

sDjListNode *list_index(sDjList *list, long index)
{
    sDjListNode* listNode = NULL;

    //  索引为正 从链表头部开始
    if(index < 0)
    {
        index = (-index) - 1;
        listNode = list ->tail;

        while(index -- && (NULL != listNode))
        {
            listNode = listNode ->pre;
        }
    }
    //  索引为负 从链表尾部开始
    else
    {
        listNode = list ->head;

        while(index -- &&(NULL != listNode))
        {
            listNode = listNode ->next;
        }
    }

    return listNode;
}

void list_rewind_head(sDjList *list, sDjListIter *iter)
{
    iter ->next = list ->head;

    iter ->direction = ITERATOR_START_HEAD;
}

void list_rewind_tail(sDjList *list, sDjListIter *iter)
{
    iter ->next = list ->tail;
    iter ->direction = ITERATOR_START_TAIL;
}

void list_rotate(sDjList *list)
{
    sDjListNode* tail = NULL;

    //  暂存表尾节点
    tail = list ->tail;
    list ->tail ->pre ->next = NULL;

    //  tail 不再有前一个节点
    tail ->pre = NULL;

    //  头结点指向原来的尾节点
    tail ->next = list ->head;
    list ->head ->pre = tail;

    list ->head = tail;
}
