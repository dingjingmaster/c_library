/*************************************************************************
> FileName: clib_sysinfo.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月24日 星期二 10时34分06秒
 ************************************************************************/
#ifndef _CLIB_SYSINFO_H
#define _CLIB_SYSINFO_H
#include "clib-ret.h"
#include <glib-object.h>

#define CLIB_TYPE_SYSINFO               (clib_sysinfo_get_type())
#define CLIB_SYSINFO(obj)               (G_TYPE_CHECK_INSTANCE_CAST((obj), CLIB_TYPE_SYSINFO, ClibSysinfo))
#define CLIB_IS_SYSINFO(obj)            (G_TYPE_CHECK_INSTANCE_TYPE((obj), CLIB_TYPE_SYSINFO))

typedef struct _ClibSysinfo             ClibSysinfo;
typedef struct _ClibSysinfoClass        ClibSysinfoClass;
typedef enum _ClibSysinfoType           ClibSysinfoType;

struct _ClibSysinfo
{
    GObject parent;
};

struct _ClibSysinfoClass
{
    GObjectClass parent_class;
};

GType clib_sysinfo_get_type (void);

/**
 * 获取 cpuinfo 键
 * @param obj: 此类
 * @param cpunum: 输出cpu数量
 * @return:
 *      成功: RET_OK;
 *      失败: RET_ERROR 错误
 */
CLIB_RET clib_sysinfo_cpuinfo_cpunum (GObject* obj, guint64* cpunum);

/**
 * CPU 总体情况
 * @param obj: 此类
 * @param hz: 输出的主频
 * @return:
 *      成功: RET_OK;
 *      失败: RET_ERROR 错误
 */
CLIB_RET clib_sysinfo_total_cpuinfo_hz (GObject* obj, guint64* hz);


/**
 * 获取指定 cpuinfo 键的数量
 * @param obj: 此类
 * @param keynum: 输出cpu数量
 * @return:
 *      成功: RET_OK;
 *      失败: RET_ERROR 错误
 */
CLIB_RET clib_sysinfo_cpuinfo_keynum (GObject* obj, guint64 cpuid, guint* keynum);

/**
 * 获取指定 cpuinfo 的键
 * @param obj: 此类
 * @param key: 输入指定键
 * @param value: 输出指定值
 * @param value_len: 输出值缓冲的大小
 * @return:
 *      成功: RET_OK;
 *      失败: RET_ERROR 错误
 */
CLIB_RET clib_sysinfo_cpuinfo_get_value_by_key (GObject* obj, guint64 cpuid, const char* key, char* value, guint64 value_len);

/**
 * 输出cpu的所有信息
 * @param obj: 此类
 * @return:
 */
void clib_sysinfo_cpuinfo_all_kv_debug (GObject* obj);


#endif
