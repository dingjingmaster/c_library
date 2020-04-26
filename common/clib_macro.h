/*************************************************************************
> FileName: clib_macro.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年04月26日 星期日 11时34分53秒
 ************************************************************************/
#ifndef _CLIB_MACRO_H
#define _CLIB_MACRO_H

/**
 * 这里把平时常用的宏添加进来
 * 删除要慎重!  (一般不可删除)
 * 修改要慎重!  (要写测试用例)
 * 添加要慎重!  (多于1条语句或很长的语句才添加到这里)
 */

#ifdef __cplusplus
extern "C" {
#endif

/* 释放 malloc 申请的内存 */
#define CT_CFREE(pointer)                                           \
    do {                                                            \
        if (NULL != pointer) free (pointer);                        \
        pointer = NULL;                                             \
    } while (0)

/* 调用函数 */
#define CT_CALL_FUNC(func, ...)                                     \
    func(##__VA_ARGS__)                                             \

/* 交换 */
#define CT_SWAP(first, second, stype)                               \
    do {                                                            \
        stype tmp = first;                                          \
        first = second;                                             \
        second = tmp;                                               \
    } while(0)

#define CT_INT_MIN(a, b, min)                                       \
        min = a > b ? b : a;

#define CT_INT_MAX(a, b, max)                                       \
        min = a > b ? a : b;

/* 成功返回 */
#define CT_RETURN_IF_SUCCESS(success)                               \
    if (success) {                                                  \
        return;                                                     \
    }

#define CT_RETURN_VALUE_IF_SUCCESS(success, val)                    \
    if (success) {                                                  \
        return val;                                                 \
    }

#ifdef __cplusplus
}
#endif
#endif
