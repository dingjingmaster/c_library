#include <stdio.h>
#include <string.h>
#include "clib_bstring.h"

//  字符串测试程序
int main(int argc, char* argv[])
{
    printf("test is beginning...\n");
    printf("test - djString_new\n");

    //  测试字符串创建完成后是否可用 C 函数操作
    printf("---------------------------------------\n");
    char* str1 = "aaaaa";
    printf(" source: |%s|\n djString: |%s|\n", str1, bstring_new(str1));
    printf("---------------------------------------\n");
    printf("\n");

    //  测试空字符串创建
    printf("---------------------------------------\n");
    char* str2 = "";
    printf(" source: |%s|\n djString: |%s|\n", str2, bstring_new(str2));
    printf("---------------------------------------\n");
    printf("\n");
    /*

    //  测试字符串释放
    printf("---------------------------------------\n");
    char* str3 = "abcdefg";
    char* tStr3 = bstring_new(str3);
    printf(" source: |%s|\n djString: |%s|\n", str3, tStr3);
    bstring_free(tStr3);
    printf(" djString_free: |%s|\n", tStr3);
    printf("---------------------------------------\n");
    printf("\n");

    //  获取字符串长度
    printf("---------------------------------------\n");
    char* str4 = "abc";
    char* tStr4 = bstring_new(str4);
    printf(" source: |%s|\n djString len: |%lu|\n", str4, bstring_len(tStr4));
    printf("---------------------------------------\n");
    printf("\n");

    //  字符串复制操作
    printf("---------------------------------------\n");
    char* tStr5 = bstring_new(str4);
    char* tStr6 = bstring_dup(tStr5);
    printf(" source: |%s|\n djString copy: |%s|\n", tStr5, tStr6);
    printf("---------------------------------------\n");
    printf("\n");

    //  请空字符串中的数据
    printf("---------------------------------------\n");
    char* tStr7 = bstring_new(str4);
    char* tStr8 = bstring_dup(tStr5);
    bstring_clear(tStr8);
    printf(" source: |%s|\n djString clear: |%s|\n", tStr7, tStr8);
    printf("---------------------------------------\n");
    printf("\n");

    //  将 char 拼接到字符串结尾
    printf("---------------------------------------\n");
    char* tStr9 = bstring_cat_str(tStr7, "I'm append string");
    printf(" source: |%s|\n djString cat: |%s|\n", tStr7, tStr9);
    printf("---------------------------------------\n");
    printf("\n");

    //  将 djString 拼接到字符串结尾
    printf("---------------------------------------\n");
    char* tStr10 = bstring_cat_str(tStr7, tStr9);
    printf(" source: |%s|\n djString djString append: |%s|\n", tStr7, tStr10);
    printf("---------------------------------------\n");
    printf("\n");

    //  复制字符串
    printf("---------------------------------------\n");
    char* tStr11 = bstring_copy_len(tStr7, "bbb", strlen("aaa"));
    printf(" source: |%s|\n djString copy: |%s|\n", tStr7, tStr11);
    printf("---------------------------------------\n");
    printf("\n");

    //  给定 char 字符串覆盖原始空间
    printf("---------------------------------------\n");
    //char* tStr12 = bstring_cover(tStr7, "cdvf");
    //printf(" source: |%s|\n djString copy: |%s|\n", tStr7, tStr12);
    //printf("---------------------------------------\n");
    //printf("\n");

    //  用空字符给字符串扩容到指定长度
    printf("---------------------------------------\n");
    char* tStr13 = bstring_grow_by_zero(tStr7, 60);
    char* tStr14 = bstring_grow_by_zero(tStr7, 5120);
    printf(" source: |%s|\n djString grow_by_zero: |%s|\n", tStr7, tStr13);
    printf(" source: |%lu|\n djString grow_by_zero: |%lu|\n", strlen(tStr7), strlen(tStr13));
    printf(" tStr7 len: |%d|\n tStr7 free grow_by_zero: |%d|\n", ((bstring_t*)((void*)tStr7 - sizeof(bstring_t))) ->len, ((bstring_t*)((void*)tStr7 - sizeof(bstring_t))) ->free);
    printf(" tStr13 len: |%d|\n tStr13 free grow_by_zero: |%d|\n", ((bstring_t*)((void*)tStr13 - sizeof(bstring_t))) ->len, ((bstring_t*)((void*)tStr13 - sizeof(bstring_t))) ->free);
    printf(" tStr14 len: |%d|\n tStr14 free grow_by_zero: |%d|\n", ((bstring_t*)((void*)tStr14 - sizeof(bstring_t))) ->len, ((bstring_t*)((void*)tStr14 - sizeof(bstring_t))) ->free);
    printf("---------------------------------------\n");
    printf("\n");

    //  保留字符串指定区间内的数据，其它数据被覆盖或者清除
    printf("---------------------------------------\n");
    char* tStr15 = bstring_new(str4);
    bstring_keep_range(tStr15, 1, 3);
    printf(" source: |%s|\n djString copy: |%s|\n", tStr4, tStr15);
    printf("---------------------------------------\n");
    printf("\n");

    //  字符串删除字符 在 str 始末位置删除 ceilStr 字符串
    printf("---------------------------------------\n");
    char* str5 = "aaaasdefdsfsdvsdhelMMMNNNSSSfsdfsdf";
    char* tStr16 = bstring_new(str5);
    char* tStr17 = bstring_strim(tStr16, "af");
    printf(" source: |%s|\n djString strim: |%s|\n", str5, tStr17);
    printf("---------------------------------------\n");
    printf("\n");

    //  字符串比较
    printf("---------------------------------------\n");
    char* str6 = "aaaasbcdef";
    char* str7 = "aaaasbcdefr";
    char* tStr18 = bstring_new(str6);
    char* tStr19 = bstring_new(str7);
    int com = bstring_compare(tStr18, tStr19);
    if(com > 0)
        puts("tStr18 greater than tStr19");
    else if(com < 0)
        puts("tStr18 less then tStr19");
    else
        puts("tStr18 equal tStr19");
    printf("---------------------------------------\n");
    */
    printf("\n");


}
