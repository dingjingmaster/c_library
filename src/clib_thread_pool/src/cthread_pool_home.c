/*************************************************************************
	> File Name: cthread_pool_home.c
	> Author: DingJing
	> Mail: dingjing_ding@staff.easou.com
	> Created Time: 2017年06月20日 星期二 10时21分21秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "cthread_pool_home.h"

//  每个线程都执行
void* cthread_pool_home_routine(void *args);

static sPthreadPool*  threadPool = NULL;
int                   worker = 50;

int cthread_pool_home_init(int num)
{
    if(num <= 0)
    {
        return RETURN_ERROR;
    }

    int               i = 0;
    void*             ret = NULL;
    int               retInt = 0;
    unsigned int      mallocSize = 0;
    pthread_t         threadTmp = 0;


    mallocSize = sizeof(sPthreadPool);

    ret = threadPool = (sPthreadPool*)malloc(mallocSize);
    if(NULL == ret)
    {
        return RETURN_ERROR;
    }

    memset(threadPool, 0, mallocSize);

    //  start
    threadPool ->maxThreadNum = num;
    threadPool ->shutDown = 0;
    threadPool ->workerNum = 0;
    threadPool ->workerHead = NULL;

    pthread_mutex_init(&(threadPool ->lock), NULL);
    pthread_cond_init(&(threadPool ->ready), NULL);

    //  create thread
    mallocSize = sizeof(pthread_t) * num;
    ret = threadPool ->threadId = (pthread_t*)malloc(mallocSize);
    if(NULL == ret)
    {
        puts("线程池内存分配错误");
        return RETURN_ERROR;
    }

    memset(threadPool ->threadId, 0, mallocSize);

    //  num thread id
    for(i = 0; i < num; ++i)
    {
        retInt = pthread_create(&(threadPool ->threadId[i]), NULL, cthread_pool_home_routine, NULL);
        if(retInt != 0)
        {
            puts("创建线程错误!!!");
            return RETURN_ERROR;
        }

    }

    return RETURN_OK;
}


void* cthread_pool_home_routine(void *args)
{
    while(1)
    {
        printf("thread %lu Created successful\n", pthread_self());

        pthread_mutex_lock(&(threadPool ->lock));

        while((threadPool ->shutDown == 0) && (threadPool ->workerNum == 0))
        {
            printf("thread %lu is waiting for work\n", pthread_self());

            pthread_cond_wait(&(threadPool ->ready), &(threadPool ->lock));
        }

        if((threadPool ->shutDown) == 1)
        {
            puts("退出了");
            pthread_mutex_unlock(&(threadPool ->lock));

            pthread_exit(NULL);
        }

        //  do work
        -- (threadPool ->workerNum);

        sPthreadWorker*  worker = (threadPool ->workerHead);
        threadPool ->workerHead = (threadPool ->workerHead) ->next;

        pthread_mutex_unlock(&(threadPool ->lock));
    
        (*(worker ->worker))(worker ->argv);

        free (worker);

        worker = NULL;
    }
}

int cthread_pool_home_add_work(cthread_worker_function workFunc, void* argv)
{
    if(NULL == workFunc)
    {
        return RETURN_ERROR;
    }

    //  
    sPthreadWorker*       workerTmp = NULL;
    sPthreadWorker*       pCur = NULL;

    workerTmp = (sPthreadWorker*) malloc (sizeof(sPthreadWorker));
    if(NULL == workerTmp)
    {
        puts("work 分配内存失败!!!");

        return RETURN_ERROR;
    }

    memset(workerTmp, 0, sizeof(sPthreadWorker));

    workerTmp ->worker = workFunc;
    workerTmp ->argv = argv;
    workerTmp ->next = NULL;

    //  lock
    pthread_mutex_lock(&(threadPool ->lock));

    pCur = threadPool ->workerHead;

    while(1)
    {
        if(NULL == (threadPool ->workerHead))
        {
            threadPool ->workerHead = workerTmp;

            break;
        }
        else if (NULL == (pCur ->next))
        {
            pCur ->next = workerTmp;

            break;
        }
        
        pCur = pCur ->next;
    }

    ++ (threadPool ->workerNum);

    pthread_mutex_unlock(&(threadPool ->lock));
    pthread_cond_signal(&(threadPool ->ready));

    return RETURN_OK;
}

int cthread_pool_home_destroy()
{
    if(1 == (threadPool ->shutDown))
    {
        return RETURN_OK;
    }

    int                   i = 0;
    sPthreadWorker*       pHead = NULL;
    sPthreadWorker*       pNext = NULL;

    threadPool ->shutDown = 1;
    
    pthread_cond_broadcast(&(threadPool ->ready));

    for(i = 0; i < (threadPool ->maxThreadNum); ++i)
    {
        pthread_join((threadPool ->threadId[i]), NULL);
    }

    if(NULL != (threadPool ->threadId))
    {
        free(threadPool ->threadId);
    }

    pHead = (threadPool ->workerHead);

    while(NULL != pHead)
    {
        pNext = pHead ->next;
        free(pHead);
        pHead = pNext ->next;
    }

    pthread_mutex_destroy(&(threadPool ->lock));
    pthread_cond_destroy(&(threadPool ->ready));

    if(NULL != threadPool)
    {
        free(threadPool);
    }
    

    return RETURN_OK;
}


