/*************************************************************************
> FileName: clib_who_example.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年11月07日 星期四 10时00分21秒
 ************************************************************************/
#include <stdio.h>
#include "clib_util.h"

int main (int argc, char* argv[])
{
    int user_num = 0;

    system_user_num (&user_num);
    printf("There are %d user logged!\n", user_num);

    system_user_num (&user_num);
    printf("There are %d user logged!\n", user_num);

    system_user_num (&user_num);
    printf("There are %d user logged!\n", user_num);

    return 0;
}
