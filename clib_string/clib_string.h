#ifndef _DJ_STRING_H_
#define _DJ_STRING_H_

#include <sys/types.h>
#include <stdarg.h>

typedef char* 	mDjStr;
typedef struct _sDjString   sDjString;

typedef struct _sDjString
{
    unsigned int     len;
    unsigned int     free;
    char   buf[];                                           //  等价于 char* 但是与 char* 不同， 地址连续与否
}sDjString;

/*  创建字符串  */
mDjStr djString_new(const char *str);

/*  创建一个空字符串  */
mDjStr djString_empty(void);

/*  释放字符串  */
void djString_free(mDjStr str);

/*  获取字符串长度  */
size_t djString_mDjStr_len(const mDjStr str);

/*  返回字符串未使用的空间字节数  */
size_t djString_avail(const mDjStr str);

/*  字符串复制操作  */
mDjStr djString_dup(const mDjStr str);

/*  清空字符串中的数据  */
void djString_clear(mDjStr str);

/*  将 char 拼接到字符串末尾  */
mDjStr djString_cat(mDjStr str, const char* cStr);

/*  将给定字符串拼接到字符串末尾  */
void djString_djString_cat();

/*  给定 char 字符串覆盖原始空间中的数据  */
void djString_cpy();

/*  用空字符给字符串扩容到指定长度  */
void djString_grow_by_zero();

/*  保留字符串指定区间内的数据，其它数据被覆盖或者清除  */
void djString_range();

/*  字符串删除字符 在 str 中删除 ceilStr 中存在的字符  */
void djString_strim(mDjStr str, const char* ceilStr);

/*  字符串比较  */
void djString_compare(const mDjStr str1, const mDjStr str2);

/*  为新的字符串分配内存  */
mDjStr djString_newLen(const void* str, size_t strLen);

/*  字符串追加  */
mDjStr djString_catLen(mDjStr str, const void* t, size_t len);

/*  为字符串增加内存, 释放了原来的字符串，创建了新的  */
mDjStr djString_expandRoom(mDjStr str, size_t addLen);




/*
 *
mDjStr djString_new_len(const void* str, size_t strLen);
//  字符串全部转化为 大写
void djString_to_upper(mDjStr str);

//  字符串全部转化为 小写
void djString_to_lower(mDjStr str);

//  字符串空间增加
mDjStr djString_enlarge_space(mDjStr str, size_t addLen);

//  字符串追加
mDjStr djString_cat_len(mDjStr str, const void* cStr, size_t len);

//  将多个字符串一次性合并到字符串中
mDjStr djString_join(char **argv, int argc, char *sep, size_t seplen);
*/

#endif
