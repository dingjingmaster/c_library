#include <stdio.h>
#include <string.h>
#include "clib_bstring.h"

int main(int argc, char* argv[])
{
    printf("test is beginning...\n");
    printf("test - bstring_new \n");

    //  字符串创建完成后是否可用 C 函数操作
    printf("---------------------------------------\n");
    char* str1 = "aaaaa";
    printf("bstring_t create\n source: |%s|\n bstring_t: |%s|\n", str1, bstring_new(str1));
    printf("---------------------------------------\n");
    printf("\n");

    //  空字符串创建
    printf("---------------------------------------\n");
    char* str2 = "";
    printf("null string test:\n source: |%s|\n bstring: |%s|\n", str2, bstring_new(str2));
    printf("---------------------------------------\n");
    printf("\n");

    //  创建指定长度字符串
    printf("---------------------------------------\n");
    char* str22 = "aaaa";
    printf("create 2 length string:\n source: |%s|\n bstring: |%s|\n", str2, bstring_new_len(str22, 2));
    printf("---------------------------------------\n");
    printf("\n");

    //  空字符串创建
    printf("---------------------------------------\n");
    printf("create null string:\n bstring: |%s|\n", bstring_empty());
    printf("---------------------------------------\n");
    printf("\n");

    //  空字符串长度
    printf("---------------------------------------\n");
    char* str3 = "aaa";
    char* str33 = bstring_new(str3);
    printf("string length:\n source: |%s|\n bstring: |%lu|\n", str2, bstring_len(str33));
    printf("---------------------------------------\n");
    printf("\n");

    // 字符串可用大小
    printf("---------------------------------------\n");
    char* str4 = "aaa";
    char* str44 = bstring_new(str4);
    str44 = bstring_new_len(str44, 10);
    printf("string length:\n source: |%s|\n bstring: |%lu|\n", str44, bstring_avail(str44));
    printf("---------------------------------------\n");
    printf("\n");

    // 字符串复制
    printf("---------------------------------------\n");
    char* str5 = bstring_dup(str44);
    printf("string dup str4:\n source: |%s|\n bstring: |%lu|\n", str5, bstring_avail(str5));
    printf("---------------------------------------\n");
    printf("\n");

    //  测试字符串释放
    printf("---------------------------------------\n");
    char* str20 = "abcdefg";
    char* tstr20 = bstring_new(str20);
    printf("test bstring free\n source: |%s|\n djString: |%s|\n", str20, tstr20);
    bstring_free(tstr20);
    printf(" djString_free: |%s|\n", tstr20);
    printf("---------------------------------------\n");
    printf("\n");

    return 0;
}
