#ifndef CLIB_LIST_H
#define CLIB_LIST_H
#include <string.h>
#include <stdlib.h>

// 从表头向表尾进行迭代
#define ITERATOR_START_HEAD 0
// 从表尾到表头进行迭代
#define ITERATOR_START_TAIL 1

//  返回链表包含节点的数量
#define LIST_LENGTH (l) ((l) ->len)
//  返回链表表头结点
#define LIST_FRIST (l) ((l) ->head)
//  返回链表表尾节点
#define LIST_TAIL (l) ((l) ->tail))

//  返回给定节点的前置节点
#define LIST_PREV_NODE (n) ((n) ->prev)
//  返回给定节点的后后置节点
#define LIST_NEXT_NODE (n) ((n) ->next)
//  返回给定节点的值
#define LIST_NODEVALUE (n) ((n) ->value)

//  将链表 l 值的复制函数设置为 m
#define LIST_SET_DUP_METHOD (l, m) ((l) ->dup = (m))
//  将链表 l 值的释放函数设置为 m
#define LIST_SET_FREE_METHOD (l, m) ((l) ->free = (m))
//  将链表 l 值的比较函数设置 为 m
#define LIST_SET_MATCH_METHOD (l, m) ((l) ->match = (m))

//  返回链表的复制函数
#define LIST_GET_DUP_METHOD (l, m) ((l) ->dup)
//  返回链表值的释放函数
#define LIST_GET_FREE_METHOD (l, m) ((l) ->free)
//  返回链表值的比较函数
#define LIST_GET_MATCH_METHOD (l, m) ((l) ->match)

typedef struct _sDjListNode sDjListNode;
typedef struct _sDjListIter sDjListIter;
typedef struct _sDjList sDjList;

//  双向链表节点
struct _sDjListNode
{
    //  前置节点
    sDjListNode*    pre;

    //  后置节点
    sDjListNode*    next;

    //  值
    void*           value;
};

//  双向链表迭代器
struct _sDjListIter
{
    //  当前迭代器的节点
    sDjListNode*    next;

    //  迭代器的方向
    int             direction;

};

//  双向链表结构
struct _sDjList
{
    //  表头结点
    sDjListNode*   head;

    //  表尾节点
    sDjListNode*   tail;

    //  节点值复制函数
    void* (*dup)(void* ptr);

    //  节点值释放函数
    void (*free) (void* ptr);

    //  节点值对比函数
    int (*match) (void* ptr, void* key);

    //  链表所含的节点数量
    unsigned long len;
};


/*  创建新的链表  */
sDjList* list_create(void);

/*  释放链表内存  */
void list_free(sDjList* list);

/*  链表头插    */
sDjList* list_addNodeHead(sDjList* list, void* value);

/*  链表尾插    */
sDjList* list_addNodeTail(sDjList* list, void* value);

/*  链表删除指定节点  */
void* list_delNode(sDjList* list, sDjListNode* delNode);

/*  链表获得迭代器 direction = 0 表示向表尾迭代， 1 表示向表头迭代 */
sDjListIter* list_getIterator(sDjList* list, int direction);

/*  链表释放迭代器 */
void list_freeIterator(sDjListIter* iter);

/*  链表下一个元素 */
sDjListNode* list_nextData(sDjListIter* iter);


/*  链表复制    */

/*  链表*/


#endif // CLIB_LIST_H
