/*************************************************************************
> FileName: clib_sysinfo.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月24日 星期二 10时34分10秒
 ************************************************************************/
#include "clib_sysinfo.h"

#define CLIB_SYSINFO_GET_PRIVATE(obj)   G_TYPE_INSTANCE_GET_PRIVATE((obj), CLIB_TYPE_SYSINFO, ClibSysinfoPrivate)

struct _ClibSysinfoPrivate
{
    gint            type;
};

enum _ClibSysinfoProperty
{
    CLIB_SYSINFO_PROPERTY_0,
    CLIB_SYSINFO_PROPERTY_TYPE,
    N_PROPERTY,
};

typedef struct _ClibSysinfoPrivate          ClibSysinfoPrivate;
typedef enum _ClibSysinfoProperty           ClibSysinfoProperty;

//G_DEFINE_TYPE (ClibSysinfo, clib_sysinfo, G_TYPE_OBJECT);
// 初始化get_type 并定义私有数据类型
G_DEFINE_TYPE_WITH_CODE (ClibSysinfo, clib_sysinfo, G_TYPE_OBJECT, G_ADD_PRIVATE (ClibSysinfo));

static void clib_sysinfo_class_init (ClibSysinfoClass* self);
static void clib_sysinfo_init (ClibSysinfo* self);
static void clib_sysinfo_get_property (GObject* obj, guint propertyid, GValue* val, GParamSpec* pspec);
static void clib_sysinfo_set_property (GObject* obj, guint propertyid, GValue* val, GParamSpec* pspec);
static void clib_sysinfo_dispose (GObject* obj);
static void clib_sysinfo_finalize (GObject* obj);

static void clib_sysinfo_class_init (ClibSysinfoClass* self)
{
    g_print ("class init\n");
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
    g_print ("init\n");
}

static void clib_sysinfo_get_property (GObject* obj, guint propertyid, GValue* val, GParamSpec* pspec)
{
    ClibSysinfo* self = CLIB_SYSINFO (obj);
    ClibSysinfoPrivate* priv = CLIB_SYSINFO_GET_PRIVATE (self);

    switch (propertyid) {
        case CLIB_SYSINFO_PROPERTY_TYPE:
            g_value_set_int (val, priv->type);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, propertyid, pspec);
    }
}

static void clib_sysinfo_set_property (GObject* obj, guint propertyid, GValue* val, GParamSpec* pspec)
{
    ClibSysinfo* self = CLIB_SYSINFO (obj);
    ClibSysinfoPrivate* priv = CLIB_SYSINFO_GET_PRIVATE (self);

    switch (propertyid) {
        case CLIB_SYSINFO_PROPERTY_TYPE:
            priv->type = g_value_get_int (val);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, propertyid, pspec);
    }
}

static void clib_sysinfo_dispose (GObject* obj)
{
    g_print ("dispose\n");
    G_OBJECT_CLASS (clib_sysinfo_parent_class)->dispose (obj);
}

static void clib_sysinfo_finalize (GObject* obj)
{
    g_print ("finalize\n");
    G_OBJECT_CLASS (clib_sysinfo_parent_class)->finalize (obj);
}


