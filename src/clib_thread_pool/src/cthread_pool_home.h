/*************************************************************************
	> File Name: cthread_pool_home.h
	> Author: DingJing
	> Mail: dingjing_ding@staff.easou.com
	> Created Time: 2017年06月20日 星期二 09时59分15秒
 ************************************************************************/

#ifndef _CTHREAD_POOL_HOME_H
#define _CTHREAD_POOL_HOME_H

#include <pthread.h>

typedef   struct _sPthreadPool    sPthreadPool;
typedef   struct _sPthreadWorker  sPthreadWorker;

typedef void* (*cthread_worker_function)(void* argv);

struct _sPthreadWorker
{
    cthread_worker_function   worker;
    void*                     argv;

    struct _sPthreadWorker*   next;
};

struct _sPthreadPool
{
    int               maxThreadNum;
    int               shutDown;
    int               workerNum;

    pthread_t*        threadId;

    sPthreadWorker*   workerHead;

    pthread_mutex_t   lock;
    pthread_cond_t    ready;
};

enum
{
    RETURN_OK = 0x00000000,
    RETURN_ERROR = 0x00000001
    
};

//  thread pool init
int cthread_pool_home_init(int num);

//  add work
int cthread_pool_home_add_work(cthread_worker_function workFunc, void* argv);

//  thread pool destroy
int cthread_pool_home_destroy();











#endif
