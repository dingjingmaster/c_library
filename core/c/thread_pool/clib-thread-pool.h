/*************************************************************************
> FileName: clib-thread-pool.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年11月26日 星期二 21时38分26秒
 ************************************************************************/

#ifndef _CLIBTHREADPOOL_H
#define _CLIBTHREADPOOL_H
#ifdef __cplusplus
extern "C" {
#endif
#include <pthread.h>
typedef struct _thread_pool_t thread_pool_t;
typedef struct _thread_worker_t thread_worker_t;

typedef void* (*thread_worker_cb) (void* arg);

/* 线程池初始化
 *
 * @param num: 线程池初始化线程数量
 * @return:
 *      失败: -1
 *      成功: 0
 */
int thread_pool_init(int num);

/* 添加工作到线程池
 *
 * @param workfunc: 线程要执行的操作
 * @param arg: 传入参数
 * @return:
 *      成功: 0
 *      失败: -1
 */
int thread_pool_add_work(thread_worker_cb workfunc, void* argv);

/* 线程池销毁
 */
void thread_pool_destory();

#ifdef __cplusplus
}
#endif
#endif
