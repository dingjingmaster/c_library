/*************************************************************************
> FileName: clib_bstring.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年11月07日 星期四 17时32分59秒
 ************************************************************************/
#ifndef _CLIB_BSTRING_H
#define _CLIB_BSTRING_H
#include <sys/types.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct _bstring_t   bstring_t;
typedef struct _bstring_t {
    unsigned int     len;
    unsigned int     free;
    char   buf[];                                   //  等价于 char* 但是与 char* 不同， 地址连续与否
}bstring_t;

/*
 * 创建字符串
 * @param str: 输入的字符串
 * @return
 *      成功: 输出字符串的字符部分
 *      失败: 返回 NULL
 */
char* bstring_new(const char *str);

/*
 * 创建一个空字符串
 * @return
 *      成功:
 *      失败:
 */
char* bstring_empty(void);

/*  释放字符串  */
void bstring_free(char* str);

/*  获取字符串长度  */
size_t bstring_len(const char* str);

/*  返回字符串未使用的空间字节数  */
size_t bstring_avail(const char* str);

/*  字符串复制操作  */
char* bstring_dup(const char* str);

/*  清空字符串中的数据  */
void bstring_clear(char* str);

/*  将 char 拼接到字符串末尾  */
char* bstring_cat_str(char* str, const char* cstr);

/*  将给定字符串拼接到字符串末尾  */
char* bstring_str_cat(char* str, const char* catstr);

/*  复制指定长度字符串   */
char* bstring_copy_len(char* str, const char* cstr, size_t cstrlen);

/*  给定 char 字符串覆盖原始空间中的数据  */
char* bstring_over(char* str, const char* cstr);

/*  用空字符给字符串扩容到指定长度  */
char* bstring_grow_by_zero(char* str, size_t len);

/*  保留字符串指定区间内的数据，其它数据被覆盖或者清除  */
void bstring_keep_range(char* str, int start, int end);

/*  字符串删除字符 在 str 始末位置删除 ceilStr 中的字符  */
char* bstring_strim(char* str, const char* ceilstr);

/*  字符串比较  */
int bstring_compare(const char* str1, const char* str2);

/*  为新的字符串分配内存  */
char* bstring_new_len(const void* str, size_t strlen);

/*  字符串追加  */
char* bstring_cat_len(char* str, const void* t, size_t len);

/*  为字符串增加内存, 释放了原来的字符串，创建了新的  */
char* bstring_expand_room(char* str, size_t addlen);


#ifdef __cplusplus
}
#endif
#endif
