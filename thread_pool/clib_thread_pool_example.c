/*************************************************************************
> FileName: clib_thread_pool_example.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年11月26日 星期二 19时23分57秒
 ************************************************************************/
#include "clib_thread_pool.h"
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t myTest;

void* print_number(void* num)
{
    pthread_mutex_lock(&myTest);
    
    long   number = 0;

    number = (long)num;

    printf("%ld\n", number);
    pthread_mutex_unlock(&myTest);

    return NULL;
}


int main()
{
    long i = 0;
    int   ret = 0;

    pthread_mutex_init(&myTest, NULL);

    ret = thread_pool_init(10);
    if(0 != ret) {
        puts("初始化错误!!!");
    }

    for (i = 0; i < 20; ++i) {
        ret = thread_pool_add_work(print_number, (void*)i);
        if(0 != ret) {
            puts("添加工作错误!!!");
        }
    }

    sleep(3);

    thread_pool_destory();
    pthread_mutex_destroy(&(myTest));

    return 0;
}

