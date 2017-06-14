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
    sDjString* sh = (sDjString*)((void*)str - sizeof(sDjString));

    return sh ->len;
}

size_t djString_avail(const mDjStr str)
{
    sDjString* sh = (sDjString*)((void*)str - sizeof(sDjString));

    return sh ->free;
}

mDjStr djString_dup(const mDjStr str)
{
    //  复制一个新的 djString
    return djString_new(str);
}

void djString_clear(mDjStr str)
{
    sDjString* sh = (sDjString* )((void*)str - sizeof(sDjString));
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
    sDjString* sh = NULL;

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
        //puts("出错了");
        return NULL;
    }

    //  获取到结构体
    sh = (sDjString*)((void*)str - sizeof(sDjString));

    //  开始追加操作
    memcpy ((void*)str + curLen, t, len);

    //  更新属性
    sh ->len = curLen;
    sh ->free = sh ->free - len;

    //  添加新的结束符号
    str[curLen + len] = '\0';

    return str;
}

//  在移动指针之前一定要把指针的步长变为 1
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

    //  str 总共需要的长度
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
    sh = (sDjString*)((void*)str - sizeof(sDjString));

    //  开始分配内存
    newSh = (sDjString*)malloc(sizeof(sDjString) + newLen + 1);
    if(NULL == newSh)
    {
        return NULL;
    }

    //  拷贝内存
    //memcpy(newSh ->buf, sh ->buf, sh ->len);
    memcpy((void*)newSh + sizeof(sDjString), sh ->buf, sh ->len);
    (newSh ->buf)[len] = '\0';

    //  更新信息
    newSh ->len = sh ->len;
    newSh ->free = newLen - newSh ->len;

    //  清除旧的字符串
    djString_free(str);

    return newSh ->buf;
}

mDjStr djString_djString_cat(mDjStr str, const mDjStr catStr)
{
    return djString_catLen (str, catStr, djString_mDjStr_len (catStr));
}

mDjStr djString_cpy(mDjStr str, const char *cStr)
{
    return djString_cpyLen (str, cStr, strlen(cStr));
}

mDjStr djString_cpyLen(mDjStr str, const char *cStr, size_t cStrLen)
{
    sDjString* sh = (sDjString*)((void*)str - sizeof(sDjString));

    //  现在长度是否足够装下 cStr
    size_t curLen = sh ->free + sh ->len;

    //  如果不够就扩展
    if(curLen < cStrLen)
    {
        str = djString_expandRoom (str, cStrLen - sh ->len);

        //  检查是否成功
        if(NULL == str)
            return NULL;

        sh = (sDjString*)((void*)str - sizeof(sDjString));
        curLen = sh ->free + sh ->len;
    }

    //  复制内容
    memcpy(str, cStr, cStrLen);

    str[cStrLen] = '\0';

    //  添加结束符号
    sh ->len = cStrLen;
    sh ->free = curLen - cStrLen;

    return str;
}

mDjStr djString_grow_by_zero(mDjStr str, size_t len)
{
    sDjString* sh = (sDjString*)((void*)str - sizeof(sDjString));
    size_t curLen = sh ->len;
    size_t toLen = 0;

    //  len 比字符串现有长度小，直接返回，不做任何处理
    if(len <= curLen)
        return str;

    //  开始扩展
    str = djString_expandRoom (str, len - curLen);

    //  检查是否成功
    if(NULL == str)
        return NULL;

    //  将新分配的内存用 0 填充
    sh = (void*)(str - sizeof(sDjString));
    memset ((void*)str + curLen, 0, (len - curLen + 1));

    //  更新属性
    toLen = sh ->len + sh ->free;
    sh ->len = len;
    sh ->free = toLen - len;

    return str;
}

void djString_range(mDjStr str, int start, int end)
{
    sDjString* sh = (sDjString*)((void*)str - sizeof(sDjString));
    size_t newLen = 0;
    size_t len = djString_mDjStr_len(str);

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

mDjStr djString_strim(mDjStr str, const char *ceilStr)
{
    //  还原字符串结构体
    sDjString* sh = (sDjString*)((void*)str - sizeof(sDjString));

    //
    char*   start = NULL;
    char*   end = NULL;
    char*   ps = NULL;
    char*   pe = NULL;

    size_t  len = 0;

    //  开始处指针
    ps = start = str;

    //  结尾处指针
    pe = end = str + djString_mDjStr_len (str);

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

int djString_compare(const mDjStr str1, const mDjStr str2)
{
    size_t len1 = djString_mDjStr_len (str1);
    size_t len2 = djString_mDjStr_len (str2);

    size_t minLen = (len1 < len2) ? len1 : len2;
    int cmp = memcmp(str1, str2, minLen);

    //  前若干字节相同，则长度之差就是结果
    if(0 == cmp)
        return len1 - len2;

    //  前若干字节比较的结果
    return cmp;
}
