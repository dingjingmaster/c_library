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

#define CT_SYSLOG(logLevel, fmt, ...) {\
    char ctsyslogBuf[2048] = {0};\
    va_list list;\
    vsnprintf(ctsyslogBuf, 2047, fmt, list);\
    va_end(list);\
    syslog_info(logLevel, __FILE__, __func__, __LINE__, ctsyslogBuf);\
}

void syslog_init(const char *category, int facility);

void syslog_info(int logLevel, const char *fileName, const char *functionName, int line, const char *info);


#ifdef __cplusplus
}
#endif
#endif
