/*************************************************************************
> FileName: gio-uri.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年06月30日 星期三 19时43分59秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>

int main (int argc, char* argv[])
{
    g_autoptr(GFile)    file = g_file_new_for_uri("file:///sss aaa kkk/sss ll");

    printf("uri:%s\n", g_file_get_uri(file));

    return 0;
}
