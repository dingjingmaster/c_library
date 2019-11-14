/*************************************************************************
> FileName: clib_bstring.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年11月07日 星期四 22时33分03秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clib_bstring.h"

char* bstring_new_len(const void* str, size_t strl)
{
    bstring_t* sh = NULL;

    if (str)
    {
        sh = malloc(sizeof(bstring_t) + strl + 1);
    }
    else
    {
        size_t len = sizeof(bstring_t) + strl + 1;      //  分配内存大小为 结构体 + 字符串长度 + 1
        sh = malloc(len);
        memset(sh, 0, len);
    }

    if(NULL == sh)
    {
        return NULL;
    }

    sh ->len = strl;
    sh ->free = 0;                                      //  新的字符串不留有空余空间

    if(str && strl)
    {
        memcpy(sh ->buf, str, strl);
    }
    sh ->buf[strl] = '\0';

    return (char*)sh ->buf;
}

char* bstring_new(const char *str)
{
    size_t strl = (NULL == str) ? 0 : strlen(str);
    return bstring_new_len(str, strl);
}

char* bstring_empty(void)
{
    return bstring_new_len("", 0);
}

void bstring_free(char* str)
{
    if(NULL == str)
        return;
    //  仅仅释放 buf 中的内容, str 之前还有字符串相关信息要一起释放, 释放后为未知内存，不能置空
    free(str - sizeof(char*));
}

size_t bstring_len(const char* str)
{
    bstring_t* sh = (bstring_t*)((void*)str - sizeof(bstring_t));
    return sh ->len;
}

size_t bstring_avail(const char* str)
{
    bstring_t* sh = (bstring_t*)((void*)str - sizeof(bstring_t));
    return sh ->free;
}

char* bstring_dup(const char* str)
{
    return bstring_new(str);
}

void bstring_clear(char* str)
{
    bstring_t* sh = (bstring_t* )((void*)str - sizeof(bstring_t));
    sh ->free += sh ->len;
    sh ->len = 0;
    sh ->buf[0] = '\0';
}

char* bstring_cat(char* str, const char *cstr) {
    return bstring_cat_len(str, cstr, strlen(cstr));
}
char* bstring_cat_len(char* str, const void* t, size_t len) {
    bstring_t* sh = NULL;

    size_t curlen = bstring_len(str);                   //  原来字符串的长度
    str = bstring_expand_room(str, len);                //  扩展字符串的长度,

    //  扩展内存可能申请新的内存，检查内存是否申请成功
    if(NULL == str) {
        return NULL;
    }

    sh = (bstring_t*)((void*)str - sizeof(bstring_t));  //  获取到结构体
    memcpy ((void*)str + curlen, t, len);               //  开始追加操作

    //  更新属性
    sh ->len = curlen;
    sh ->free = sh ->free - len;

    //  添加新的结束符号
    str[curlen + len] = '\0';

    return str;
}

//  在移动指针之前一定要把指针的步长变为 1
char* bstring_expand_room(char* str, size_t addlen) {
    bstring_t* sh = NULL;
    bstring_t* newsh = NULL;
    size_t free = bstring_avail(str);                   //  获取目前剩余空间长度

    size_t len = 0;                                     //  目前使用的长度
    size_t newlen = 0;

    if(free >= addlen) {                                //  判断剩余空间是否大于要扩展的内存
        return str;
    }

    len = bstring_len(str);                             //  如果内存不够用，扩展
    newlen = (len + addlen);                            //  str 总共需要的长度

    /*  根据新的长度，确定为 str 分配的新的长度 */
    if(newlen < 1024) {                                 //  小于 1k 分配两倍的长度
        newlen *= 2;
    } else {                                            //  否则加上 0.5k
        newlen += 512;
    }
    sh = (bstring_t*)((void*)str - sizeof(bstring_t));  //  获取现有字符串的信息
    newsh = (bstring_t*)malloc(sizeof(bstring_t) + newlen + 1);
    if(NULL == newsh) {
        return NULL;
    }

    memcpy((void*)newsh + sizeof(bstring_t), sh ->buf, sh ->len);
    (newsh ->buf)[len] = '\0';

    newsh ->len = sh ->len;
    newsh ->free = newlen - newsh ->len;

    bstring_free(str);

    return newsh ->buf;
}

char* bstring_cat_str(char* str, const char* catstr) {
    return bstring_cat_len (str, catstr, bstring_len (catstr));
}

char* bstring_copy(char* str, const char *cstr) {
    return bstring_copy_len (str, cstr, strlen(cstr));
}

char* bstring_copy_len(char* str, const char *cstr, size_t cstrlen) {
    bstring_t* sh = (bstring_t*)((void*)str - sizeof(bstring_t));

    //  现在长度是否足够装下 cStr
    size_t curlen = sh ->free + sh ->len;

    //  如果不够就扩展
    if(curlen < cstrlen)
    {
        str = bstring_expand_room (str, cstrlen - sh ->len);

        //  检查是否成功
        if(NULL == str)
            return NULL;

        sh = (bstring_t*)((void*)str - sizeof(bstring_t));
        curlen = sh ->free + sh ->len;
    }

    //  复制内容
    memcpy(str, cstr, cstrlen);

    str[cstrlen] = '\0';

    //  添加结束符号
    sh ->len = cstrlen;
    sh ->free = curlen - cstrlen;

    return str;
}

char* bstring_grow_by_zero(char* str, size_t len)
{
    bstring_t* sh = (bstring_t*)((void*)str - sizeof(bstring_t));
    size_t curLen = sh ->len;
    size_t toLen = 0;

    //  len 比字符串现有长度小，直接返回，不做任何处理
    if(len <= curLen)
        return str;

    //  开始扩展
    str = bstring_expand_room (str, len - curLen);

    //  检查是否成功
    if(NULL == str)
        return NULL;

    //  将新分配的内存用 0 填充
    sh = (void*)(str - sizeof(bstring_t));
    memset ((void*)str + curLen, 0, (len - curLen + 1));

    //  更新属性
    toLen = sh ->len + sh ->free;
    sh ->len = len;
    sh ->free = toLen - len;

    return str;
}

void bstring_keep_range(char* str, int start, int end)
{
    bstring_t* sh = (bstring_t*)((void*)str - sizeof(bstring_t));
    size_t newLen = 0;
    size_t len = bstring_len(str);

    //  空字符串，不需要执行清除操作
    if(0 == len)
        return;

    //  start 小于 0 考虑从字符串末尾开始保留字符串
    if(start < 0)
    {
        start += len;
        if(start < 0)
            start = 0;
    }

    //  end 小于 0 考虑从字符串末尾开始保留字符串
    if(end < 0)
    {
        end += len;
        if(end < 0)
            end = 0;
    }

    //  start < end 时获得要保留的字符串的范围(正常情况)
    newLen = (start > end) ? 0 : (end - start + 1);

    //  start > end 如果 start 大于 end 则 start = 0
    if(newLen != 0)
    {
        if(start >= (signed)len)
        {
            newLen = 0;
        }
        else if(end >= (signed)len)
        {
            end = len - 1;
            newLen = (start > end) ? 0 : (end - start + 1);
        }
    }
    else
    {
        start = 0;
    }

    //  对字符串进行移动 start = 0 不需要移动 start > 0 且 要保留的字符串长度大于 0 则移动
    if(start && newLen)
        memmove (sh ->buf, sh ->buf + start, newLen);

    //  添加终结符 相当于把要保留的字符都放到的前边，其它字符删除。
    sh ->buf[newLen] = 0;

    //  更新属性
    sh ->free = sh ->free + (sh ->len - newLen);
    sh ->len = newLen;
}

char* bstring_strim(char* str, const char *ceilStr) {
    //  还原字符串结构体
    bstring_t* sh = (bstring_t*)((void*)str - sizeof(bstring_t));

    //
    char*   start = NULL;
    char*   end = NULL;
    char*   ps = NULL;
    char*   pe = NULL;

    size_t  len = 0;

    //  开始处指针
    ps = start = str;

    //  结尾处指针
    pe = end = str + bstring_len (str);

    //  开始去除字符
    while(ps <= end && strchr (ceilStr, *ps))
        ++ ps;

    while(pe > start && strchr(ceilStr, *pe))
        -- pe;

    //  计算 trim 之后的长度
    len = (ps > pe) ? 0 : (pe - ps + 1);

    //  可能的字符前移操作
    if(sh ->buf != ps)
        memmove (sh ->buf, ps, len);

    //  添加终止字符
    sh ->buf[len] = '\0';

    //  更新属性
    sh ->free = sh ->free + (sh ->len - len);
    sh ->len = len;

    return str;
}

int bstring_compare(const char* str1, const char* str2) {
    size_t len1 = bstring_len (str1);
    size_t len2 = bstring_len (str2);

    size_t minLen = (len1 < len2) ? len1 : len2;
    int cmp = memcmp(str1, str2, minLen);

    //  前若干字节相同，则长度之差就是结果
    if(0 == cmp)
        return len1 - len2;

    //  前若干字节比较的结果
    return cmp;
}
