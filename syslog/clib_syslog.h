#ifndef CLIB_SYSLOG_H
#define CLIB_SYSLOG_H
#include <stdio.h>
#include <stdarg.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief syslog日志输出宏函数
 * @param logLevel 日志输出级别
 * @param ... 可变参数列表
 */
#define CT_SYSLOG(logLevel,...) syslog_info(logLevel, __FILE__, __func__, __LINE__, ##__VA_ARGS__);

/**
 * @brief 日志参数初始化
 * @param category 标签
 * @param facility 设备文件
 * @retval void
 */
void syslog_init(const char *category, int logLevel, int facility);

/**
 * @brief 日志输出到system log, 默认LOG_INFO级别
 * @param loglevel 日志级别
 * @param file 文件名
 * @param function 函数
 * @param line 函数
 * @param fmt 格式化字符串
 * @retval void
 */
void syslog_info(int logLevel, const char *file, const char *function, int line, const char* fmt, ...);

#ifdef __cplusplus
}
#endif
#endif
