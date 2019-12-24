/*************************************************************************
> FileName: clib_sysinfo.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月24日 星期二 10时34分06秒
 ************************************************************************/
#ifndef _CLIB_SYSINFO_H
#define _CLIB_SYSINFO_H
#include <glib-object.h>

#define CLIB_TYPE_SYSINFO               (clib_sysinfo_get_type())
#define CLIB_SYSINFO(obj)               (G_TYPE_CHECK_INSTANCE_CAST((obj), CLIB_TYPE_SYSINFO, ClibSysinfo))
#define CLIB_IS_SYSINFO(obj)            (G_TYPE_CHECK_INSTANCE_TYPE((obj), CLIB_TYPE_SYSINFO))

typedef struct _ClibSysinfo             ClibSysinfo;
typedef struct _ClibSysinfoClass        ClibSysinfoClass;

struct _ClibSysinfo
{
    GObject parent;
};

struct _ClibSysinfoClass
{
    GObjectClass parent_class;
};

GType clib_sysinfo_get_type (void);

#endif
