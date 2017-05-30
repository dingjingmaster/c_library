#ifndef _DJ_STRING_H_
#define _DJ_STRING_H_

#include <sys/types.h>
#include <stdarg.h>

typedef char* 	mDjStr;
typedef struct _sDjString   sDjString;

struct _sDjString
{
    unsigned int     len;
    unsigned int     free;
    char   buf[];                                           //  等价于 char* 但是与 char* 不同， 地址连续与否
}sDjString;


mDjStr djString_new_len(const void* str, size_t strLen);

//  获取字符串长度
size_t djString_mDjStr_len(const mDjStr str);

//  字符串初始化并分配内存
mDjStr djString_new(const char *str);

//  获取空字符串
mDjStr djString_empty(void);

//  字符串复制操作
mDjStr djString_dup(const mDjStr str);

//  释放
void djString_free(mDjStr str);

//  字符串删除字符 在 str 中删除 ceilStr 中存在的字符, 并去除空白
void djString_strim(mDjStr str, const char* ceilStr);

//  字符串全部转化为 大写
void djString_to_upper(mDjStr str);

//  字符串全部转化为 小写
void djString_to_lower(mDjStr str);

//  字符串比较
void djString_compare(const mDjStr str1, const mDjStr str2);

//  字符串空间增加
mDjStr djString_enlarge_space(mDjStr str, size_t addLen);

//  字符串追加
mDjStr djString_cat_len(mDjStr str, const void* cStr, size_t len);

//  将多个字符串一次性合并到字符串中
mDjStr djString_join(char **argv, int argc, char *sep, size_t seplen);


#endif