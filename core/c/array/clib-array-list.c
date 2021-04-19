/*************************************************************************
> FileName: clib-array-list.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年07月24日 星期三 16时45分35秒
 ************************************************************************/
#include "clib-array-list.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct _array_list_node_t array_list_node_t;

struct _array_list_node_t {
    array_value_t           data;
    array_list_node_t*      next;
    array_list_node_t*      prev;
};

struct _array_list_t {
    array_list_node_t*      head;       // 数据根节点
    array_list_node_t*      end;        // 数据末尾节点
    unsigned long           length;     // 已用空间
    unsigned long           cap;        // 总空间
    array_list_value_cmp    cmp_func;   // 比较两个值的大小
};


/* 初始化 */
array_list_t* array_list_new(unsigned long cap, array_list_value_cmp cb) {
    unsigned long i = 0;
    array_list_t*           array = NULL;
    array_list_node_t**     pc = NULL;
    array_list_node_t*      prev = NULL;
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
        prev = new_node;
        new_node = (array_list_node_t*) malloc (sizeof(array_list_node_t));
        if(NULL == new_node) {
            array->cap = i;
            break;
        }
        new_node->data = NULL;
        new_node->next = NULL;
        new_node->prev = NULL;
        new_node->prev = prev;
        *pc = new_node;
        pc = &((*pc)->next);
    }
    array->end = new_node;

    return array;
}

/* 销毁数组 */
void array_list_free(array_list_t** array) {
    array_list_node_t*      pc = (*array)->end;
    while (NULL != pc) {
        free(pc);
        pc = pc->prev;
    }
    free(*array);
    *array = NULL;
}

/* 追加元素 */
array_value_t array_list_append(array_list_t* array, array_value_t value) {
    if (NULL == value || NULL == array) {
        return NULL;
    }
    unsigned long           i = 0;
    unsigned long           length = array->length;
    array_list_node_t*      new_node = NULL;
    array_list_node_t*      pc = array->head; // 可优化，由末未开始还是头开始遍历

    if(length < array->cap) {
        for(i = 0; i < length; ++i) {
            pc = pc->next;
        }
        pc->data = value;
        array->length += 1;
    } else {
        new_node = (array_list_node_t*)malloc(sizeof(array_list_node_t));
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
    }
    return value;
}

unsigned int array_list_length(array_list_t* array) {
    if (NULL == array) {
        return 0;
    }
    return array->length;
}

void array_list_print(array_list_t* array, array_list_print_cb print) {
    if (NULL == array || NULL == print) {
        return;
    }
    unsigned long           i = 0;
    array_list_node_t*      pc = array->head;

    for (i = 0; i < array->length; ++i) {
        print(pc->data);
        pc = pc->next;
    }
}
