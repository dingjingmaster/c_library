/*************************************************************************
> FileName: gio-create.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年03月20日 星期六 11时35分54秒
 ************************************************************************/

#include <glib.h>
#include <fcntl.h>
#include <gio/gio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <glib/gstdio.h>

int main (int argc, char* argv[])
{
    GStatBuf buf;

    g_stat (argv[1], &buf);


    return 0;
}
