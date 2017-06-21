/*************************************************************************
	> File Name: clib_log.h
	> Author: DingJing
	> Mail: dingjing_ding@staff.easou.com
	> Created Time: 2017年06月16日 星期五 17时00分39秒
 ************************************************************************/

#ifndef _CLIB_LOG_H
#define _CLIB_LOG_H

/*  日志 路径长度 和 每条日志输出长度   */
#define   CLIB_LOG_LINE_SIZE    4096


#define   CLIB_LOG_LEVEL_EMERG    0
#define   CLIB_LOG_LEVEL_ALERT    1
#define   CLIB_LOG_LEVEL_CRIT     2
#define   CLIB_LOG_LEVEL_ERR      3
#define   CLIB_LOG_LEVEL_WARNING  4
#define   CLIB_LOG_LEVEL_NOTICE   5
#define   CLIB_LOG_LEVEL_INFO     6
#define   CLIB_LOG_LEVEL_DEBUG    7

/*  */
#define   DEBUG(fmt, ...)   CLIB_LOG_PRINTF(7, fmt, ##__VA_ARGS__)
#define   INFO(fmt, ...)    CLIB_LOG_PRINTF(6, fmt, ##__VA_ARGS__)
#define   WARN(fmt, ...)    CLIB_LOG_PRINTF(4, "WARN", fmt, ##__VA_ARGS__)
#define   ERROR(fmt, ...)   CLIB_LOG_PRINTF(3, "ERROR", fmt, ##__VA_ARGS__)
#define   FATAL(fmt, ...)   CLIB_LOG_PRINTF(2, "FATAL", fmt, ##__VA_ARGS__)


void clib_log_init(const char* path, uint64_t max_size, uint32_t keep_num, uint32_t level);






#endif
