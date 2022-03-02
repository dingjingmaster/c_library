/*************************************************************************
> FileName: gio-loop-start.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2022年02月28日 星期一 13时33分54秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>

int main (int argc, char* argv[])
{
    if (argc < 2) {
        printf ("请输入要挂载的 CD 设备名!\n");
        return -1;
    }

    const char*     uri = argv[1];

    g_autofree (GFile) file = g_file_new_for_uri (uri);
    g_return_val_if_fail (G_IS_FILE(file), -1);

    g_autofree (GError) error = NULL;
    g_autofree (GMount) mount = g_file_find_enclosing_mount (file, NULL, &error);
    if (!error) {
        printf ("error: %s\n", error->message);
        return -1;
    }



    return 0;
}
