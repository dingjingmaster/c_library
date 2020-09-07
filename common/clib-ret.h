/*************************************************************************
> FileName: clib_ret.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月06日 星期五 10时12分17秒
 ************************************************************************/
#ifndef _CLIBRET_H
#define _CLIBRET_H

/*  定义返回值 */
typedef enum bool
{
    RET_OK,                                                             // 正常
    RET_ERROR,                                                          // 错误

    RET_TRUE,                                                           // 真
    RET_FALSE,                                                          // 假
    RET_NOTENOUGHMEM,                                                   // 存储不够
} CLIB_RET;

/* 定义全局常量 */
#define CLIB_SYSTEM_ETC_PASSWD                          "/etc/passwd"
#define CLIB_SYSTEM_PROC                                "/proc/"


#endif
