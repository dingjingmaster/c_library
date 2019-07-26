/*************************************************************************
> FileName: clib_array_list.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年07月24日 星期三 16时45分35秒
 ************************************************************************/
#include "clib_array_list.h"
#include <stdlib.h>

typedef struct _array_list_node_t array_list_node_t;

struct _array_list_node_t {
    array_value_t           data;
    array_list_node_t*      next;
    array_list_node_t*      prev;
};

struct _array_list_t {
    array_list_node_t*      head;       // 数据根节点
    array_list_node_t*      end;        // 数据末尾节点
    unsigned int            length;     // 已用空间
    unsigned int            cap;        // 总空间
    array_list_value_cmp    cmp_func;   // 比较两个值的大小
};


/* 初始化 */
array_list_t* array_list_new(unsigned long cap, array_list_value_cmp cb) {
    int i = 0;
    array_list_t*           array = NULL;
    array_list_node_t**     pc = NULL;
    array_list_node_t*      new_node = NULL;

    array = (array_list_t*)malloc(sizeof(array_list_t));
    if (NULL == array) {
        return NULL;
    }

    array->head = NULL;
    array->end = NULL;
    array->length = 0;
    array->cap = cap <= 0 ? 100 : cap;
    array->cmp_func = cb;

    pc = &(array->head);
    for(i = 0; i < array->cap; ++i) {
        new_node = (array_list_node_t*) malloc (sizeof(array_list_node_t));
        if(NULL == new_node) {
            array->cap = i;
            break;
        }
        new_node->data = NULL;
        new_node->next = NULL;
        new_node->prev = NULL;

        // 如果不是头结点
        if (array->head != *pc) {
            new_node->prev = *pc;
        }
        *pc = new_node;
        pc = &((*pc)->next);
    }
    array->end = *pc;

    return array;
}

/* 追加元素 */
array_value_t array_list_append(array_list_t* array, array_value_t value) {
    if (NULL == value || NULL == array) {
        return NULL;
    }

    unsigned int            i = 0;
    unsigned int            length = array->length;
    array_list_node_t*      pc = array->head; // 可优化，由末未开始还是头开始遍历

    if(length + 1 > array->cap) {
        array_list_node_t* new_node = (array_list_node_t*)malloc(sizeof(array_list_node_t));
        if(NULL == new_node) {
            return NULL;
        }
        new_node->data = value;
        new_node->next = NULL;
        new_node->prev = array->end;
        array->end->next = new_node;
        array->end = new_node;
        array->length += 1;
        array->cap += 1;
    } else {
//        puts("ok\n");
        for(i = 0; i < length; ++i) {
            pc = pc->next;
        }
        pc->data = value;
        array->length += 1;
    }
    return value;
}
