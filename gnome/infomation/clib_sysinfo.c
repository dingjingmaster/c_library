/*************************************************************************
> FileName: clib_sysinfo.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月24日 星期二 10时34分10秒
 ************************************************************************/
#include "clib_sysinfo.h"
#include <stdio.h>
#include <string.h>
#include <glibtop/sysinfo.h>

#define CLIB_SYSINFO_GET_PRIVATE(obj)   G_TYPE_INSTANCE_GET_PRIVATE((obj), CLIB_TYPE_SYSINFO, ClibSysinfoPrivate)

struct _ClibSysinfoPrivate
{
    void;
};

enum _ClibSysinfoProperty
{
    CLIB_SYSINFO_PROPERTY_0,
    CLIB_SYSINFO_PROPERTY_TYPE,
    N_PROPERTY,
};

typedef struct _ClibSysinfoPrivate          ClibSysinfoPrivate;
typedef enum _ClibSysinfoProperty           ClibSysinfoProperty;

G_DEFINE_TYPE (ClibSysinfo, clib_sysinfo, G_TYPE_OBJECT);
// 初始化get_type 并定义私有数据类型

//G_DEFINE_TYPE_WITH_CODE (ClibSysinfo, clib_sysinfo, G_TYPE_OBJECT, G_ADD_PRIVATE (ClibSysinfo));

static void clib_sysinfo_class_init (ClibSysinfoClass* self);
static void clib_sysinfo_init (ClibSysinfo* self);
static void clib_sysinfo_get_property (GObject* obj, guint propertyid, GValue* val, GParamSpec* pspec);
static void clib_sysinfo_set_property (GObject* obj, guint propertyid, GValue* val, GParamSpec* pspec);
static void clib_sysinfo_dispose (GObject* obj);
static void clib_sysinfo_finalize (GObject* obj);

static void clib_sysinfo_class_init (ClibSysinfoClass* self)
{
    GParamSpec* pspec[N_PROPERTY] = {NULL,};
    
    GObjectClass* base_class = G_OBJECT_CLASS (self);

    base_class->get_property = clib_sysinfo_get_property;
    base_class->set_property = clib_sysinfo_set_property;
    base_class->dispose = clib_sysinfo_dispose;
    base_class->finalize = clib_sysinfo_finalize;

    pspec[CLIB_SYSINFO_PROPERTY_TYPE] = g_param_spec_int ("type", "Type", "whitch type information", 0, G_MAXINT, 0, G_PARAM_READWRITE);

    g_object_class_install_properties (base_class, N_PROPERTY, pspec);
}

static void clib_sysinfo_init (ClibSysinfo* self)
{
    ClibSysinfoPrivate* priv = CLIB_SYSINFO_GET_PRIVATE (self);
}

static void clib_sysinfo_get_property (GObject* obj, guint propertyid, GValue* val, GParamSpec* pspec)
{
    ClibSysinfo* self = CLIB_SYSINFO (obj);
    ClibSysinfoPrivate* priv = CLIB_SYSINFO_GET_PRIVATE (self);

#if 0
    switch (propertyid) {
        case CLIB_SYSINFO_PROPERTY_TYPE:
            g_value_set_int (val, priv->type);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, propertyid, pspec);
    }
#endif
}

static void clib_sysinfo_set_property (GObject* obj, guint propertyid, GValue* val, GParamSpec* pspec)
{
    ClibSysinfo* self = CLIB_SYSINFO (obj);
    ClibSysinfoPrivate* priv = CLIB_SYSINFO_GET_PRIVATE (self);

#if 0
    switch (propertyid) {
        case CLIB_SYSINFO_PROPERTY_TYPE:
            priv->type = g_value_get_int (val);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, propertyid, pspec);
    }
#endif
}

CLIB_RET clib_sysinfo_cpuinfo_cpunum (GObject* obj, guint64* cpunum)
{
    if (NULL == cpunum || NULL == obj) return RET_ERROR;

    ClibSysinfo* self = CLIB_SYSINFO (obj);
    ClibSysinfoPrivate* priv = CLIB_SYSINFO_GET_PRIVATE (self);

    glibtop_sysinfo* sysinfo = glibtop_get_sysinfo();
    *cpunum = sysinfo->ncpu;

    return RET_OK;
}

CLIB_RET clib_sysinfo_total_cpuinfo_hz (GObject* obj, guint64* hz)
{
    if (NULL == hz || NULL == obj) return RET_ERROR;
    glibtop_cpu buf;
    glibtop_get_cpu (&buf);

    *hz = buf.frequency;

    return RET_OK;
}

CLIB_RET clib_sysinfo_cpuinfo_keynum (GObject* obj, guint64 cpuid, guint* keynum)
{
    if (NULL == keynum || NULL == obj) return RET_ERROR;

    ClibSysinfo* self = CLIB_SYSINFO (obj);
    ClibSysinfoPrivate* priv = CLIB_SYSINFO_GET_PRIVATE (self);

    glibtop_sysinfo* sysinfo = glibtop_get_sysinfo();
    if (sysinfo->ncpu < cpuid) return RET_ERROR;

    *keynum = sysinfo->cpuinfo[cpuid].labels->len;

    return RET_OK;
}

CLIB_RET clib_sysinfo_cpuinfo_get_value_by_key (GObject* obj, guint64 cpuid, const char* key, char* value, guint64 value_len)
{
    if (NULL == key || NULL == obj || NULL == value) return RET_ERROR;

    int i = 0;
    char* ikey = NULL;
    ClibSysinfo* self = CLIB_SYSINFO (obj);
    ClibSysinfoPrivate* priv = CLIB_SYSINFO_GET_PRIVATE (self);

    glibtop_sysinfo* sysinfo = glibtop_get_sysinfo();
    if (sysinfo->ncpu < cpuid) return RET_ERROR;

    memset (value, 0, value_len);

    for (i = 0; i < sysinfo->cpuinfo[cpuid].labels->len; ++i) {
        ikey = g_ptr_array_index (sysinfo->cpuinfo[cpuid].labels, i);
        if (0 == strcmp (key, ikey)) {
            snprintf (value, value_len - 1, "%s", g_hash_table_lookup(sysinfo->cpuinfo[cpuid].values, ikey));
            return RET_OK;
        }
    }

    return RET_ERROR;
}

void clib_sysinfo_cpuinfo_all_kv_debug (GObject* obj)
{
    ClibSysinfo* self = CLIB_SYSINFO (obj);
    ClibSysinfoPrivate* priv = CLIB_SYSINFO_GET_PRIVATE (self);

    glibtop_sysinfo* sysinfo = glibtop_get_sysinfo();
    printf ("flags:%d\n", sysinfo->flags);
    printf ("ncpu:%d\n", sysinfo->ncpu);
    printf ("======================\n");
    for (int i = 0; i < sysinfo->ncpu; ++i) {
        printf ("\n");
        for (int j = 0; j < sysinfo->cpuinfo[i].labels->len; ++j) {
            char* ci = g_ptr_array_index (sysinfo->cpuinfo[i].labels, j);
            printf ("==>%s --- %s\n", ci, g_hash_table_lookup(sysinfo->cpuinfo[i].values, ci));
        }
        printf ("\n");
    }
}

static void clib_sysinfo_dispose (GObject* obj)
{
    ClibSysinfo* self = CLIB_SYSINFO (obj);
    ClibSysinfoPrivate* priv = CLIB_SYSINFO_GET_PRIVATE (self);


    G_OBJECT_CLASS (clib_sysinfo_parent_class)->dispose (obj);
}

static void clib_sysinfo_finalize (GObject* obj)
{
    ClibSysinfo* self = CLIB_SYSINFO (obj);
    ClibSysinfoPrivate* priv = CLIB_SYSINFO_GET_PRIVATE (self);

    G_OBJECT_CLASS (clib_sysinfo_parent_class)->finalize (obj);
}


