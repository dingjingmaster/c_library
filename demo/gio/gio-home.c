/*************************************************************************
> FileName: gio-home.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年01月19日 星期二 19时57分35秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>


int main (int argc, char* argv)
{
    printf ("home: %s\n", g_get_home_dir());

    return 0;
}


