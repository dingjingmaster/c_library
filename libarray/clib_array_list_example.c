/*************************************************************************
> FileName: clib_array_list_example.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年07月24日 星期三 17时57分39秒
 ************************************************************************/
#include <stdio.h>

#include "clib_array_list.h"

int main(void) {
    array_list_t*       array;
    array_value_t*      value;

    array = array_list_new(100, NULL);
    if(NULL == array) {
        puts("array_list_new 初始化失败!");
        return -1;
    }

    for (int i = 0; i < 500; ++i) {
        int v = i;
        value = array_list_append(array, &v);
        if(NULL == value) {
            puts("array_list_append 失败!");
            return -1;
        }
    }

    return 0;
}
