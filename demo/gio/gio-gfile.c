/*************************************************************************
> FileName: gio-gfile.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月13日 星期五 15时03分32秒
 ************************************************************************/
#include <gio/gio.h>
#include <stdio.h>

int main (void)
{
    GFile* file = NULL;
    GFile* file1 = NULL;

    // 创建文件 g_file_new_for_uri /comdline/tmp/
    file = g_file_new_for_path ("test.test");

    file1 = g_file_parse_name ("/data/code/a/b/a.txt");

    g_object_unref (file1);
    g_object_unref (file);
}
