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

#define CT_SYSLOG(logLevel, ...) {\
    syslog_info(logLevel, __FILE__, __func__, __LINE__, __VA_ARGS__);\
}

void syslog_init(const char *category, int facility);

void syslog_info(int logLevel, const char *fileName, const char *functionName, int line, char* fmt, ...);


#ifdef __cplusplus
}
#endif
#endif
