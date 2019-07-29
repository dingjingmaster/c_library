/*************************************************************************
> FileName: clib_array_list_example.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年07月24日 星期三 17时57分39秒
 ************************************************************************/
#include <stdio.h>

#include "clib_array_list.h"

void print_v (array_value_t v) {
    printf("%d  ", *((int*)v));
}

int main(void) {
    array_list_t*       array;
    array_value_t*      value;
    int                 values[] = {
            0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
            10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
            20, 21, 22, 23, 24, 25, 26, 27, 28, 29
    };

    array = array_list_new(10, NULL);
    if(NULL == array) {
        puts("array_list_new 初始化失败!");
        return -1;
    }

    printf("长度：%d", array_list_length(array));

    for (int i = 0; i < sizeof(values) / sizeof(int); ++i) {
        value = array_list_append(array, (array_value_t)&(values[i]));
        if(NULL == value) {
            puts("array_list_append 失败!");
            return -1;
        }
    }

    printf("\n");
    array_list_print(array, print_v);
    printf("\n");
    printf("长度：%d\n", array_list_length(array));
    printf("\n");
    array_list_free(&array);

    return 0;
}
