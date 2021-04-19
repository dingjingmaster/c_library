/*************************************************************************
> FileName: clib-double-list.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年11月07日 星期四 21时25分38秒
 ************************************************************************/
#ifndef _CLIBDOUBLELIST_H
#define _CLIBDOUBLELIST_H
#ifdef __cplusplus
extern "C"{
#endif

typedef struct _double_list_node_t double_list_node_t;
typedef struct _double_list_iter_t double_list_iter_t;
typedef struct _double_list_t double_list_t;

//  双向链表节点
struct _double_list_node_t {
    double_list_node_t* pre;                                //  前置节点
    double_list_node_t* next;                               //  后置节点
    void*               value;                              //  值
};

//  双向链表迭代器
struct _double_list_iter_t {
    double_list_node_t* next;                               //  当前迭代器的节点
    int                 direction;                          //  迭代器的方向
};

//  双向链表结构
struct _double_list_t {
    double_list_node_t* head;                               //  表头结点
    double_list_node_t* tail;                               //  表尾节点
    void* (*dup)(void* ptr);                                //  节点值复制函数
    void (*free) (void* ptr);                               //  节点值释放函数
    int (*match) (void* ptr, void* key);                    //  节点值对比函数
    unsigned long len;                                      //  链表所含的节点数量
};

#define ITERATOR_START_HEAD 0                               // 从表头向表尾进行迭代
#define ITERATOR_START_TAIL 1                               // 从表尾到表头进行迭代

#define LIST_LENGTH(l) ((l) ->len)                          //  返回链表包含节点的数量
#define LIST_FRIST(l) ((l) ->head)                          //  返回链表表头结点
#define LIST_TAIL(l) ((l) ->tail)                           //  返回链表表尾节点

#define LIST_PREV_NODE (n) ((n) ->prev)                     //  返回给定节点的前置节点
#define LIST_NEXT_NODE(n) ((n) ->next)                      //  返回给定节点的后后置节点
#define LIST_NODEVALUE(n) ((n) ->value)                     //  返回给定节点的值

#define LIST_SET_DUP_METHOD(l, m) ((l) ->dup = (m))         //  将链表 l 值的复制函数设置为 m
#define LIST_SET_FREE_METHOD(l, m) ((l) ->free = (m))       //  将链表 l 值的释放函数设置为 m
#define LIST_SET_MATCH_METHOD(l, m) ((l) ->match = (m))     //  将链表 l 值的比较函数设置 为 m

#define LIST_GET_DUP_METHOD(l, m) ((l) ->dup)               //  返回链表的复制函数
#define LIST_GET_FREE_METHOD(l, m) ((l) ->free)             //  返回链表值的释放函数
#define LIST_GET_MATCH_METHOD(l, m) ((l) ->match)           //  返回链表值的比较函数

/*  创建新的链表  */
double_list_t* list_create(void);

/*  释放链表内存  */
void list_free(double_list_t* list);

/*  链表头插    */
double_list_t* list_add_node_head(double_list_t* list, void* value);

/*  链表尾插    */
double_list_t* list_add_node_tail(double_list_t* list, void* value);

/*  链表删除指定节点  */
void list_del_node(double_list_t* list, double_list_node_t* delNode);

/*  链表获得迭代器 direction = 0 表示向表尾迭代， 1 表示向表头迭代 */
double_list_iter_t* list_get_iterator(double_list_t* list, int direction);

/*  链表释放迭代器 */
void list_free_iterator(double_list_iter_t* iter);

/*  链表下一个元素 */
double_list_node_t* list_next_data(double_list_iter_t* iter);

/*  链表复制    */
double_list_t* list_dup_list(double_list_t* origList);

/*  查找链表 list 中值与 value 匹配的节点   */
double_list_node_t* list_search_value(double_list_t* list, void* value);

/*  返回链表给定索引位置的节点    */
double_list_node_t* list_index(double_list_t* list, long index);

/*  将迭代器置到链表的开头位置   */
void list_rewind_head(double_list_t* list, double_list_iter_t* iter);

/*  将迭代器置到链表的结尾位置   */
void list_rewind_tail(double_list_t* list, double_list_iter_t* iter);

/*  取出表尾节点放到表头，成为新的链表   */
void list_rotate(double_list_t* list);


#ifdef __cplusplus
}
#endif
#endif
