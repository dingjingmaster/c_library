#include "dj_string.h"

/*      字符串初始化并分配内存        */
mDjStr djString_new_len(const void* str, size_t strLen)
{
    sDjString*              djStr;                              //  并不是指向该结构体，而是 buf

    if(str)                                                     //  是不是空指针
    {
        djStr = malloc(sizeof sDjString + strLen + 1);          //  直接分配 12 + strLen 长度的内存
    }
    else
    {
        djStr = calloc(sizeof sDjString + strLen + 1, 1);
    }

    if(djStr == NULL)                                           //  内存分配失败
    {
        return NULL;
    }

    djStr ->len = strLen;
    djStr ->free = 0;

    if(str && strLen)                                           //  数据拷贝
    {
        memcpy(djStr ->buf, str, strLen);
    }

    djStr ->buf[strLen] = '\0';

    return (char*)djStr ->buf;
}

/*      获取字符串的长度        */
static inline size_t djString_len(const mDjStr str)               //  为了利于使用 s 是执行 sDjString 结构体的 buf[]
{
    sDjString* str = (sDjString *)(str - sizeof sDjString);

    return str ->len;
}

/*      初始化字符串          */
mDjStr djString_new(const char *str)
{
    size_t strLen = (str == NULL) ? 0 : strlen(str);

    return djString_new_len(str, strLen);
}

/*      返回一个空字符串        */
mDjStr djString_empty(void)
{
    return djString_new_len("", 0);
}

/*      复制字符串             */
mDjStr djString_dup(const mDjStr str)
{
    return djString_new_len(str, djString_len(str));
}

/*      释放                  */
void djString_free(mDjStr str)
{
    if(NULL == str)
    {
        return;
    }

    free(s - sizeof sDjString);
}

/*      字符删除操作          */
void djString_strim(mDjStr str, const char* ceilStr)
{
    char*           start = NULL;
    char*           end = NULL;
    char*           ps = NULL;
    char*           pe = NULL;
    size_t          len = 0;
    sDjString* strTmp = (str - sizeof sDjString);                       //  一般是小端对齐 str 指的就是 bufs

    head = ps = strTmp;
    end = pe = strTmp + djString_mDjStr_len(str) - 1;

    while(ps <= end && strchr(ceilStr, *ps))
        ps ++;
    
    while(pe > start && strchr(ceilStr, *pe))
        pe --;

    len = (ps < pe) ? ((pe - ps) + 1) : 0

    if(strTmp ->buf != ps)
        memmove(strTmp ->buf, ps, len);

    strTmp ->len = len;
}

/*      字符串全部转化为 大写         */
void djString_to_upper(mDjStr str)
{
    int i = 0;
    int len = djString_len(str);

    for(i = 0; i < len; ++i)
        str[i] = toupper(s[i]);
}

/*      字符串全部转化为 小写         */
void djString_to_lower(mDjStr str)
{
    int i = 0;
    int len = djString_len(str);

    for(i = 0; i < len; ++i)
        str[i] = tolower(s[i]);
}

/*      字符串比较       */
void djString_compare(const mDjStr str1, const mDjStr str2)
{
    int         cmp = 0;
    int         strLen1 = djString_len(str1);
    int         strLen2 = djString_len(str2);
    int         minStr =  strLen1 < strLen2 ? strLen1 : strLen2;

    cmp = memcmp(str1, str2, minStr);

    if (cmp == 0)
        return strLen1 - strLen2;

    return cmp;
}

mDjStr djString_enlarge_space(mDjStr str, size_t addLen)
{
    
}


/*      字符串追加       */
/*
mDjStr djString_cat_len(mDjStr str, const void* cStr, size_t len)
{
    sDjString           sh;
    size_t              curLen = djString_len(str);
}
*/

/*      将多个字符串一次性合并到字符串中        */
//mDjStr djString_join(char **argv, int argc, char *sep, size_t seplen);






































