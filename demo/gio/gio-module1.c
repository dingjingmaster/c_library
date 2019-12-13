/*************************************************************************
> FileName: gio1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月13日 星期五 11时41分17秒
 ************************************************************************/
#include <gio/gio.h>

int main (void)
{
    GIOModule* giom = NULL;

    giom = g_io_module_new ("text");
    return 0;
}
