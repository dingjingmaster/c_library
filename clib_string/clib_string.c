#include <stdio.h>
#include <stdlib.h>
#include "clib_string.h"

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

////////////////////////////////////////////////////////////        API         /////////////////////////////////////////////////////////////////////////
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

}
