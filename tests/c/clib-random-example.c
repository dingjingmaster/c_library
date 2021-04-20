/*************************************************************************
> FileName: clib-random-example.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年10月12日 星期一 14时40分55秒
 ************************************************************************/
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../../core/c/random/clib-random.h"

int main (int argc, char* argv[])
{
    uint32_t arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (int i = 0; i < sizeof(arr) / sizeof(uint32_t); ++i) {
        printf ("%d ", arr[i]);
    }

    printf ("\n === \n");

    random_shuffle((int)time(NULL), arr, sizeof (arr) / sizeof (uint32_t));

    for (int i = 0; i < sizeof(arr) / sizeof(uint32_t); ++i) {
        printf ("%d ", arr[i]);
    }

    printf ("\n");

    return 0;
}
