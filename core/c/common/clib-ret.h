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

/**
 * 退出返回值
 */
#define DJCLIB_EFAILURE         1                                       // 失敗
#define DJCLIB_ESUCCESS         0                                       // 成功

/**
 * 成功返回值
 */
#define DJCLIB_DONE             9                                       // already done
#define DJCLIB_DCLOSE           8                                       // error response, close socket
#define DJCLIB_CLOSE            7                                       // normal response, close socket
#define DJCLIB_TIMEOUT          6                                       // timeout, but got response
#define DJCLIB_DROP             5                                       // data dropped
#define DJCLIB_APPEND           4                                       // insert do not replace old data
#define DJCLIB_REPLACE          3                                       // insert replace old data
#define DJCLIB_OVFL             2                                       // overflow or busy or full
#define DJCLIB_NFOUND           1                                       // search result, not found.
#define DJCLIB_OK               0

#define DJCLIB_TRUE             1                                       // boolean return code, true.
#define DJCLIB_FALSE            0                                       // boolean return code, false.

/**
 * 失败返回值
 */
#define DJCLIB_ERROR            (-1)
#define DJCLIB_EEXIT            (-1001)                                 // critical error, should exit.
#define DJCLIB_EBUG             (-1002)                                 // programming error
#define DJCLIB_EMEM             (-1003)                                 // memory error
#define DJCLIB_EINVAL           (-1004)                                 // invalid input
#define DJCLIB_EOVFL            (-1005)                                 // overflow
#define DJCLIB_EBUSY            (-1006)                                 // busy
#define DJCLIB_EFILE            (-1007)                                 // file operation error
#define DJCLIB_ECALL            (-1008)                                 // system call error
#define DJCLIB_EAGAIN           (-1009)                                 // EINTR or EAGAIN, retry later.
#define DJCLIB_ETIMEOUT         (-1010)                                 // socket timeout
#define DJCLIB_ECLOSED          (-1011)                                 // socket already closed
#define DJCLIB_ECLOSE           (-1012)                                 // should close connection

/* 定义全局常量 */
#define CLIB_SYSTEM_ETC_PASSWD  "/etc/passwd"
#define CLIB_SYSTEM_PROC        "/proc/"


#endif
