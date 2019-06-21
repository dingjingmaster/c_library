#ifndef CLIB_LOG_H
#define CLIB_LOG_H

/**
 * full_path_name = _log_path/_log_name.log
 * _log_path = /path/of/file
 * _log_name = _log_name_prefix _log_name_time ._log_name_suffix
 *
 */
#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    LOG_EMERG           =   0,              /* 系统不兼容 */
    LOG_ALERT           =   1,              /* 严重错误 */
    LOG_CRIT            =   2,              /* 严重警告 */
    LOG_ERR             =   3,              /* 错误 */
    LOG_WARNING         =   4,              /* 警告 */
    LOG_NOTICE          =   5,              /* 正常但重要的情况 */
    LOG_INFO            =   6,              /* 信息 */
    LOG_DEBUG           =   7,              /* 调试信息 */
    LOG_VERB            =   8,              /* 详细信息 */
} log_level_t;

typedef enum {
    LOG_ROTATE_TRUE     =   1,              /* 允许分文件 */
    LOG_ROTATE_FALSE    =   2,              /* 不允许分文件 */
} log_rotate_t;


/**
 * @param type: 输出类型
 */
int log_init(log_level_t level, log_rotate_t rotate, int log_size,
             const char* dir, const char* prefix, const char* suffix);

/**
 * 销毁
 */
void log_destroy(void);

/**
 * 设置 log 的级别
 * @param level: log 级别
 *
 */
void log_set_level(int level);

/**
 * 日志大小
 * @param size: log 大小
 */
void log_set_split_size(int size);

/**
 * 是否允许 log 切分
 * @param enable:
 */
void log_set_rotate(int enable);

/**
 * 设置 log 存储路径
 *
 * @param path: 存储路径
 * @return 失败: -1; 成功: 0
 */
int log_set_path(const char* path);

/**
 *
 *
 */
int log_print(int level, const char* tag, const char* file, int line, const char* func, const char* fmt, ...);


#ifdef __cplusplus
}
#endif

#endif // CLIB_LOG_H
