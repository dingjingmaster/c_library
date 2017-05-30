#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clib_string.h"

/*      创建新的字符串     */
mDjStr djString_new(const char *str)
{
    size_t strLen = (NULL == str) ? 0 : strlen(str);

    return djString_newLen(str, strLen);
}

mDjStr djString_empty(void)
{
    return djString_newLen("", 0);
}

void djString_free(mDjStr str)
{
    if(NULL == str)
    {
        return;
    }

    //  仅仅释放 buf 中的内容, str 之前还有字符串相关信息要一起释放, 释放后为未知内存，不能置空
    free(str - sizeof(sDjString));
}

size_t djString_mDjStr_len(const mDjStr str)
{
    sDjString* sh = (sDjString*)(str - sizeof(sDjString));

    return sh ->len;
}

size_t djString_avail(const mDjStr str)
{
    sDjString* sh = (sDjString*)(str - sizeof(sDjString));

    return sh ->free;
}

mDjStr djString_dup(const mDjStr str)
{
    //  复制一个新的 djString
    return djString_new(str);
}

void djString_clear(mDjStr str)
{
    sDjString* sh = (sDjString* )(str - sizeof(sDjString));
    sh ->free += sh ->len;
    sh ->len = 0;
    sh ->buf[0] = '\0';
}

mDjStr djString_cat(mDjStr str, const char *cStr)
{
    return djString_catLen(str, cStr, strlen(cStr));
}

mDjStr djString_newLen(const void* str, size_t strLen)
{
    sDjString*     sh = NULL;

    //  分配内存
    if (str)
    {
        //  传入了空 NULL
        sh = malloc(sizeof(sDjString) + strLen + 1);
    }
    else
    {
        //  分配内存大小为 结构体 + 字符串长度 + 1
        size_t len = sizeof(sDjString) + strLen + 1;
        sh = malloc(len);
        memset(sh, 0, len);
    }

    //  如果内存分配失败则返回
    if(NULL == sh)
    {
        return NULL;
    }

    //  设置初始值
    sh ->len = strLen;

    //  新的字符串不留有空余空间
    sh ->free = 0;

    //  将数据复制进去
    if(str && strLen)
    {
        memcpy(sh ->buf, str, strLen);
    }

    //  结尾
    sh ->buf[strLen] = '\0';

    //  返回 buf 部分
    return (char*)sh ->buf;
}

mDjStr djString_catLen(mDjStr str, const void* t, size_t len)
{
    sDjString* sh = NULL;

    //  原来字符串的长度
    size_t curLen = djString_mDjStr_len(str);

    //  扩展字符串的长度,
    str = djString_expandRoom(str, len);

    //  扩展内存可能申请新的内存，检查内存是否申请成功
    if(NULL == str)
    {
        return NULL;
    }

    //  获取到结构体
    sh = (sDjString*)(str - sizeof(sDjString));

    //  开始追加操作
    memcpy (str + curLen, t, len);

    //  更新属性
    sh ->len = curLen;
    sh ->free = sh ->free - len;

    //  添加新的结束符号
    str[curLen + len] = '\0';

    return str;
}

mDjStr djString_expandRoom(mDjStr str, size_t addLen)
{
    sDjString* sh = NULL;
    sDjString* newSh = NULL;

    //  获取目前剩余空间长度
    size_t free = djString_avail(str);

    //  目前使用的长度
    size_t len = 0;
    size_t newLen = 0;

    //  判断剩余空间是否大于要扩展的内存
    if(free >= addLen)
    {
        return str;
    }

    //  如果内存不够用，扩展
    len = djString_mDjStr_len(str);
    sh = (sDjString*)(str - (sizeof(sDjString)));

    //  str 还需要的长度
    newLen = (len + addLen);

    //  根据新的长度，确定为 str 分配的新的长度
    if(newLen < 1024)               //  小于 1k 分配两倍的长度
    {
        newLen *= 2;
    }
    else                            //  否则加上 0.5k
    {
        newLen += 512;
    }

    //  获取现有字符串的信息
    sh = (sDjString*)(str - sizeof(sDjString));

    //  开始分配内存
    newSh = (sDjString*)malloc(sizeof(sDjString) + newLen + 1);
    if(NULL == newSh)
    {
        return NULL;
    }

    //  拷贝内存
    memcpy(newSh + sizeof(sDjString), sh ->buf, sh ->len);

    //  更新信息
    newSh ->len = sh ->len;
    newSh ->free = newLen - newSh ->len;

    //  清除旧的字符串
    djString_free(str);

    return newSh ->buf;
}
