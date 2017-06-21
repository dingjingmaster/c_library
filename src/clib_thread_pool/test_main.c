/*************************************************************************
	> File Name: test_main.c
	> Author: DingJing
	> Mail: dingjing_ding@staff.easou.com
	> Created Time: 2017年06月21日 星期三 09时39分21秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include "./src/cthread_pool_home.h"

pthread_mutex_t myTest;

void* print_number(void* num)
{
    pthread_mutex_lock(&myTest);
    
    long   number = 0;

    number = (long)num;

    printf("%ld\n", number);
    pthread_mutex_unlock(&myTest);
}


int main()
{
    long i = 0;
    int   ret = 0;

    pthread_mutex_init(&myTest, NULL);

    ret = cthread_pool_home_init(10);
    if(RETURN_OK != ret)
    {
        puts("初始化错误!!!");
    }

    for (i = 0; i < 20; ++i)
    {
        ret = cthread_pool_home_add_work(print_number, (void*)i);
        if(RETURN_OK != ret)
        {
            puts("添加工作错误!!!");
        }
    }

    sleep(10000);

    cthread_pool_home_destroy();
    pthread_mutex_destroy(&(myTest));

    return 0;
}

