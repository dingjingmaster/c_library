/*************************************************************************
> FileName: clib_sysinfo_examplt.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月24日 星期二 10时52分29秒
 ************************************************************************/
#include "clib_sysinfo.h"

int main (void)
{
    ClibSysinfo* sys = NULL;
    guint keynum = 0;
    guint64 cpunum = 0;
    char* value[100] = {0};

    g_type_init ();

    sys = g_object_new (CLIB_TYPE_SYSINFO, NULL);
    if (!G_IS_OBJECT(sys)) {
        g_print ("this is not a gobject!\n");
        return -1;
    }

    clib_sysinfo_cpuinfo_all_kv_debug (sys);
    clib_sysinfo_cpuinfo_get_value_by_key (sys, 7, "model name", value, sizeof (value));
    clib_sysinfo_cpuinfo_cpunum (sys, &cpunum);
    clib_sysinfo_cpuinfo_keynum (sys, 7, &keynum);

    printf ("cpus:%d\n", cpunum);
    printf ("keys:%d\n", keynum);
    printf ("key: model name --- value:%s\n", value);

    g_object_unref (sys);
}
