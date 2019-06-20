#ifndef CLIB_LOG_H
#define CLIB_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#define         LOG_EMERG           0   /* 系统不兼容 */
#define         LOG_ALERT           1   /* 严重错误 */
#define         LOG_CRIT            2   /* 严重警告 */
#define         LOG_ERR             3   /* 错误 */
#define         LOG_WARNING         4   /* 警告 */
#define         LOG_NOTICE          5   /* 正常但重要的情况 */
#define         LOG_INFO            6   /* 信息 */
#define         LOG_DEBUG           7   /* 调试信息 */
#define         LOG_VERB            8   /* 详细信息 */

#define         LOG_OUTPUT_FILE     0   /* log 文件输出 */
#define         LOG_OUTPUT_CONSOLE  1   /* log 控制台输出 */

typedef enum {
    LOG_STDIN       =   0,              /* 标准输入 */
    LOG_STDOUT      =   1,              /* 标准输出 */
    LOG_STDERR      =   2,              /* 标准错误输出 */
    LOG_FILE        =   3,              /* 文件 */
    LOG_RSYSLOG     =   4,

    LOG_MAX_OUTPUT  =   255,
} log_type_t;


/**
 * @param type: 输出类型
 */
int log_init(int type, const char* ident);

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
