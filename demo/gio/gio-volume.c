/*************************************************************************
> FileName: gio-volume.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月14日 星期六 13时12分46秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>

// 遍历连接到电脑的驱动器
void driver_func (gpointer data, gpointer udata)
{
    GDrive* v = (GDrive*) data;
    printf ("name:%s\n", g_drive_get_name(v));
    

}

// 磁盘/卷 监控
int main (int argc, char* argv[])
{
    GVolumeMonitor* vm = NULL;
    GList* drives = NULL;

    vm = g_volume_monitor_get();
    if (NULL == vm) {
        printf ("错误!");
        return -1;
    }

    drives = g_volume_monitor_get_connected_drives (vm);
    if (NULL == drives) {
        printf ("返回错误");
        return -1;
    }
    printf ("drives num:%d\n", g_list_length(drives));
    printf ("=========== 已连接的卷 ============\n");
    g_list_foreach (drives, driver_func, NULL);
    printf ("===================================\n");


    
    // 元素释放
    g_list_free (drives);
    g_object_unref (vm);

    return 0;
}
