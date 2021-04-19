/*************************************************************************
> FileName: clib-conf.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年10月12日 星期一 11时32分28秒
 ************************************************************************/
#ifndef _CLIB_CONF_H
#define _CLIB_CONF_H
#include <stdint.h>

#define CLIB_CONF_MAX_LINE_BYTES                4096

/**
 * @brief 读取配置文件
 * @param  conf: 用于保存读取到的配置文件
 * @param  path_name: 配置文件的路径
 * @param  buf_size: conf 的大小
 * @param  kv_size: 配置文件的kv键值对数量
 */
int conf_load(void *conf, const char *path_name, uint32_t buf_size, uint32_t kv_asize);

const char *conf_value(void *conf, const char *key, const char *def);
uint64_t conf_udec(void *conf, const char *key, uint64_t def);
uint64_t conf_uhex(void *conf, const char *key, uint64_t def);

void conf_dump(void *conf);

#endif
