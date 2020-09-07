/*************************************************************************
> FileName: clib-double-list.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年11月07日 星期四 21时25分50秒
 ************************************************************************/
#include <stdlib.h>

#include "clib-double-list.h"

double_list_t* list_create() {
    double_list_t*   list = NULL;
    if((list = (double_list_t*)malloc(sizeof(double_list_t))) == NULL) {
        return NULL;
    }

    list ->head = NULL;
    list ->tail = NULL;
    list ->len = 0;
    list ->dup = NULL;
    list ->free = NULL;
    list ->match = NULL;

    return list;
}

void list_free(double_list_t* list) {
    double_list_node_t* pcur = NULL;
    double_list_node_t* pnext = NULL;

    //  指向头指针
    pcur = list ->head;

    while(--(list ->len)) {
        pnext = pcur ->next;
        if(NULL != (list ->free))
            list ->free(pcur ->value);
        free(pcur);
        pcur = pnext;
    }

    list ->head = NULL;
    list ->tail = NULL;
    list ->dup = NULL;
    list ->free = NULL;
    list ->match = NULL;
}

double_list_t* list_add_node_head(double_list_t* list, void* value) {
    double_list_node_t* node = NULL;

    if((node = (double_list_node_t*)malloc(sizeof(double_list_node_t))) == NULL) {
        return NULL;
    }

    node ->value = value;
    if(0 == list ->len) {
        list ->head = node;
        list ->tail= node;
        node ->next = NULL;
        node ->pre = NULL;
    } else {
        node ->next = list ->head;
        list ->head ->pre = node;
        list ->head = node;
        node ->pre = NULL;
    }

    list ->len ++;
    return list;
}

double_list_t* list_add_node_tail(double_list_t* list, void *value) {
    double_list_node_t* newNode = NULL;

    if((newNode = (double_list_node_t*)malloc(sizeof(double_list_node_t))) == NULL) {
        return NULL;
    }

    newNode ->value = value;
    if(0 == list ->len) {
        list ->head = newNode;
        list ->tail = newNode;
        newNode ->pre = NULL;
        newNode ->next = NULL;
    } else {
        list ->tail ->next = newNode;
        newNode ->pre = list ->tail;
        list ->tail = newNode;
        newNode ->next = NULL;
    }

    list ->len ++;
    return list;
}

void list_del_node(double_list_t*list, double_list_node_t* delNode) {
    if(NULL != delNode ->pre) {
        delNode ->pre ->next = delNode ->next;
    } else {
        list ->head = delNode ->next;
    }

    if(NULL != delNode ->next) {
        delNode ->next ->pre = delNode ->pre;
    } else {
        list ->tail = delNode ->pre;
    }

    if(list ->free)
        list ->free(delNode ->value);
    free(delNode);

    list ->len --;
}

double_list_iter_t* list_get_iterator(double_list_t* list, int direction) {
    double_list_iter_t* iter;
    if((iter = (double_list_iter_t*)malloc(sizeof(double_list_iter_t))) == NULL)
        return NULL;

    if(direction == ITERATOR_START_HEAD)
        iter ->next = list ->head;
    else if(direction == ITERATOR_START_TAIL)
        iter ->next = list ->tail;
    else
        return NULL;

    iter ->direction = direction;

    return iter;
}

void list_free_iterator(double_list_iter_t* iter) {
    if(NULL == iter)
        return;
    iter ->next = NULL;
    free(iter);
}

double_list_node_t* list_next_data(double_list_iter_t* iter) {
    double_list_node_t* current = iter ->next;
    if(NULL != current) {
        if(iter ->direction == ITERATOR_START_HEAD)
            iter ->next = current ->next;
        else if(iter ->direction == ITERATOR_START_TAIL)
            iter ->next = current ->pre;
    }
    return current;
}

double_list_t* list_dup_list(double_list_t* orig) {
    double_list_t* copyList = NULL;
    double_list_iter_t* iter = NULL;
    double_list_node_t* listNode = NULL;

    if((copyList = list_create ()) == NULL) {
        return NULL;
    }

    //  给新的链表设置处理函数
    copyList ->dup = orig ->dup;
    copyList ->free = orig ->free;
    copyList ->match = orig ->match;

    //  迭代链表并把值加入到新的链表中
    iter = list_get_iterator (orig, ITERATOR_START_HEAD);
    while (NULL != (listNode = list_next_data (iter))) {
        void* value = NULL;
        if(copyList ->dup) {
            value = copyList ->dup(listNode ->value);
            if(NULL == value) {
                list_free(copyList);
                list_free_iterator(iter);
                return NULL;
            }
        } else {
            value = listNode ->value;
        }

        if(NULL == list_add_node_tail (copyList, value)) {
            list_free(value);
            list_free_iterator(iter);

            return NULL;
        }
    }
    list_free_iterator(iter);

    return copyList;
}

double_list_node_t* list_search_value(double_list_t* list, void *value) {
    double_list_iter_t*    iter = NULL;
    double_list_node_t*    listNode = NULL;

    iter = list_get_iterator (list, ITERATOR_START_HEAD);

    while(NULL != (listNode = list_next_data (iter))) {
        if(list ->match) {
            if(list ->match(listNode ->value, value)) {
                list_free_iterator(iter);
                return listNode;
            }
        } else {
            if(value == listNode ->value) {
                list_free_iterator(iter);
                return listNode;
            }
        }
    }

    list_free_iterator(iter);

    return NULL;
}

double_list_node_t* list_index(double_list_t* list, long index) {
    double_list_node_t* listNode = NULL;

    if(index < 0) {
        index = (-index) - 1;
        listNode = list ->tail;

        while(index -- && (NULL != listNode)) {
            listNode = listNode ->pre;
        }
    } else {
        listNode = list ->head;
        while(index -- &&(NULL != listNode)) {
            listNode = listNode ->next;
        }
    }
    return listNode;
}

void list_rewind_head(double_list_t* list, double_list_iter_t* iter) {
    iter ->next = list ->head;
    iter ->direction = ITERATOR_START_HEAD;
}

void list_rewind_tail(double_list_t* list, double_list_iter_t* iter) {
    iter ->next = list ->tail;
    iter ->direction = ITERATOR_START_TAIL;
}

void list_rotate(double_list_t* list) {
    double_list_node_t* tail = NULL;

    tail = list ->tail;
    list ->tail ->pre ->next = NULL;
    tail ->pre = NULL;

    tail ->next = list ->head;
    list ->head ->pre = tail;

    list ->head = tail;
}
