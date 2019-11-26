/*************************************************************************
> FileName: clib_thread_pool.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年11月26日 星期二 21时13分45秒
 ************************************************************************/
#include "clib_thread_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _thread_worker_t
{
    thread_worker_cb            worker;
    void*                       arg;
    struct _thread_worker_t*    next;
};

struct _thread_pool_t 
{
    int                         threadnum;
    int                         shutdown;
    int                         workernum;
    pthread_t*                  threadid;
    thread_worker_t*            workerlist;
    pthread_mutex_t             lock;
    pthread_cond_t              ready;
};

static void* thread_pool_routine(void* arg);

static thread_pool_t*           threadpool = NULL;

int thread_pool_init (int num)
{
    if (num <= 0) {
        return -1;
    }

    int             i = 0;
    int             ret = 0;

    threadpool = (thread_pool_t*) malloc (sizeof(thread_pool_t));
    if (NULL == threadpool) {
        return -1;
    }

    memset (threadpool, 0, sizeof(thread_pool_t));

    // 初始化线程池参数
    threadpool->threadnum = num;
    threadpool->shutdown = 0;
    threadpool->workernum = 0;
    threadpool->workerlist = NULL;

    // 初始化锁和条件变量
    pthread_mutex_init(&(threadpool->lock), NULL);
    pthread_cond_init(&(threadpool->ready), NULL);

    // 创建线程
    threadpool->threadid = (pthread_t*) malloc (sizeof(pthread_t) * num);
    if (NULL == threadpool->threadid) {
        free (threadpool);
        return -1;
    }
    memset(threadpool->threadid, 0, sizeof(pthread_t) * num);

    for (i = 0; i < num; ++i) {
        ret = pthread_create(&(threadpool->threadid[i]), NULL, thread_pool_routine, NULL);
        if (0 != ret) {
            free (threadpool->threadid);
            free (threadpool);
            return -1;
        }
    }

    return 0;
}

static void* thread_pool_routine(void* arg)
{
    while (1) {
        pthread_mutex_lock (&(threadpool->lock));
        while ((0 == threadpool->shutdown) && (0 == threadpool->workernum)) {
            pthread_cond_wait(&(threadpool->ready), &(threadpool->lock));
        }

        if (1 == threadpool->shutdown) {
            pthread_mutex_unlock (&(threadpool->lock));
            pthread_exit (NULL);
        }
        -- threadpool->workernum;
        thread_worker_t* worker = threadpool->workerlist;
        threadpool->workerlist = threadpool->workerlist->next;
        pthread_mutex_unlock (&(threadpool->lock));
        (*(worker->worker))(worker->arg);
        free (worker);
        worker = NULL;
    }
}

int thread_pool_add_work (thread_worker_cb workerfunc, void* arg)
{
    if (NULL == workerfunc) return -1;

    thread_worker_t*            workertmp = NULL;
    thread_worker_t*            curworker = NULL;

    workertmp = (thread_worker_t*) malloc (sizeof(thread_worker_t));
    if (NULL == workertmp) {
        return -1;
    }

    memset (workertmp, 0, sizeof(thread_worker_t));

    workertmp->worker = workerfunc;
    workertmp->arg = arg;
    workertmp->next = NULL;

    pthread_mutex_lock (&(threadpool->lock));
    curworker = threadpool->workerlist;

    while (1) {
        if (NULL == threadpool->workerlist) {
            threadpool->workerlist = workertmp;
            break;
        } else if (NULL == curworker->next) {
            curworker->next = workertmp;
            break;
        }
        curworker = curworker->next;
    }
    ++ threadpool->workernum;
    pthread_mutex_unlock (&(threadpool->lock));
    pthread_cond_signal (&(threadpool->ready));

    return 0;
}

void thread_pool_destory ()
{
    if (1 == threadpool->shutdown) {
        return;
    }
    int                 i = 0;
    thread_worker_t*    phead = NULL;
    thread_worker_t*    pnext = NULL;

    threadpool->shutdown = 1;
    pthread_cond_broadcast (&(threadpool->ready));

    for (i = 0; i < threadpool->threadnum; ++i) {
        pthread_join (threadpool->threadid[i], NULL);
    }

    if (NULL != threadpool->threadid) {
        free (threadpool->threadid);
    }

    phead = threadpool->workerlist;

    while (NULL != phead) {
        pnext = phead->next;
        if (NULL == pnext) break;
        free (phead);
        phead = pnext->next;
    }
    pthread_mutex_destroy (&(threadpool->lock));
    pthread_cond_destroy (&(threadpool->ready));

    if (NULL != threadpool) {
        free (threadpool);
    }
}
