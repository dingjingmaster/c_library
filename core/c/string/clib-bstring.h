/*************************************************************************
> FileName: clib_bstring.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年11月07日 星期四 22时32分59秒
 ************************************************************************/
#ifndef _CLIBBSTRING_H
#define _CLIBBSTRING_H
#include <sys/types.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct _bstring_t   bstring_t;

typedef struct _bstring_t
{
    unsigned int     len;
    unsigned int     free;
    char   buf[];                                   //  等价于 char* 但是与 char* 不同， 地址连续与否
} bstring_t;

/*
 * 为新的字符串分配内存
 * @param str: 字符串首地址
 * @param strlen: 长度
 * @return
 *      成功: 输出字符串部分首地址
 *      失败：返回NULL
 *
 */
char* bstring_new_len(const void* str, size_t strlen);

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
 *      成功: 返回长度为0的空字符串首地址
 *      失败: NULL
 */
char* bstring_empty(void);

/*
 * 释放字符串buffuer中的内容
 * @param str: 字符串
 * @return
 *      无
 */
void bstring_free(char* str);

/*
 * 获取字符串长度
 * @param str: bstring_t 类型
 * @return 
 *      字符串长度
 */
size_t bstring_len(const char* str);

/*
 * 返回字符串未使用的空间字节数
 * @param str: bstring_t类型字符串
 * @return
 *      可用空余长度（会自动增加）
 */
size_t bstring_avail(const char* str);

/*
 * 字符串复制操作
 * @param str: bstring_t
 * @return
 *      成功: bstring_t
 *      失败: NULL
 */
char* bstring_dup(const char* str);

/*
 * 清空字符串中的数据
 * @param str: bstring_t
 * @return
 *      void
 */
void bstring_clear(char* str);

/*
 * 将 char 拼接到字符串末尾
 * @param str: bstring_t
 * @param cstr: 要追加的字符串
 * @return
 *      成功: bstring_t
 *      失败: NULL
 */
char* bstring_cat_str(char* str, const char* cstr);

/*
 * 覆盖指定长度字符串(cstr会覆盖str,cstrlen是str总长度)
 * @param str: bstring_t
 * @param cstr: c字符串
 * @param cstrlen: 字符串总长度
 * @return
 *      正确：复制得到的
 *      失败：NULL
 */
char* bstring_cover_len(char* str, const char* cstr, size_t cstrlen);

/*
 * 给定 char 字符串覆盖原始空间中的数据
 * @param str: bstring_t
 * @param cstr: c字符串
 * @return
 *      正确: bstring_t
 *      错误: NULL
 */
char* bstring_cover(char* str, const char* cstr);

/*
 * 用空字符给字符串扩容到指定长度
 * @param str: bstring_t
 * @param len: 指定长度
 * @return
 *      正确: bstring_t
 *      错误: NULL
 */
char* bstring_grow_by_zero(char* str, size_t len);

/*
 * 保留字符串指定区间内的数据，其它数据被覆盖或者清除
 * @param str: bstring_t
 * @param start: 开始位置
 * @param end: 结束位置
 * @return
 *      void
 */
void bstring_keep_range(char* str, int start, int end);

/*
 * 字符串删除字符 在 str 始末位置删除 ceilStr 中的字符
 * @param str: bstring_t
 * @param ceilstr: 要匹配删除的字符串
 * @return
 *      正确: bstring_t
 *      错误: NULL
 */
char* bstring_strim(char* str, const char* ceilstr);

/*  
 * 字符串比较
 * @param str1: bstring_t
 * @param str2: bstring_t
 * @return
 *      相等: 0
 *      大于0: str1 长于 str2 或 str1 memcmp > 0
 */
int bstring_compare(const char* str1, const char* str2);

/*
 * 字符串追加(可以追加二进制串)
 * @param str: bstring_t
 * @param t: 要追加的字符串
 * @param len: 要追加的长度
 * @return
 *      正确: bstring_t
 *      错误: NULL
 */
char* bstring_cat_len(char* str, const void* t, size_t len);

/*
 * 为字符串增加内存, 释放了原来的字符串，创建了新的
 * @param str: bstring_t
 * @param addlen: 要增加的长度
 * @return
 *      正常: bstring_t
 *      错误: NULL*/
char* bstring_expand_room(char* str, size_t addlen);

#ifdef __cplusplus
}
#endif
#endif
