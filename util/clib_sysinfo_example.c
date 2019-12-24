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

    g_type_init ();

    sys = g_object_new (CLIB_TYPE_SYSINFO, "type", 0, NULL);
    if (!G_IS_OBJECT(sys)) {
        g_print ("this is not a gobject!\n");
        return -1;
    }

    g_object_unref (sys);
}
