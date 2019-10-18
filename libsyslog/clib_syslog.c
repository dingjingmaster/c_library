#include "clib_syslog.h"

static char sysCategory[128] = {0};
static int sysFacility = 0;

void syslog_init(const char *category, int facility) {
    if (NULL == category) {
        return;
    }

    memset(sysCategory, 0, sizeof sysCategory);
    strncpy(sysCategory, category, sizeof sysCategory - 1);
    sysFacility = facility;
}

void syslog_info(int logLevel, const char *fileName, const char *functionName, int line, const char* info) {
    char buf[2048] = {0};
    char *logLevelstr = "";

    memset(buf, 0, sizeof buf);

    openlog("", LOG_NDELAY, sysFacility);
    switch (logLevel) {
    case LOG_EMERG:
        logLevelstr = "EMERG";
        break;
    case LOG_ALERT:
        logLevelstr = "ALERT";
        break;
    case LOG_CRIT:
        logLevelstr = "CRIT";
        break;
    case LOG_ERR:
        logLevelstr = "ERROR";
        break;
    case LOG_WARNING:
        logLevelstr = "WARNING";
        break;
    case LOG_NOTICE:
        logLevelstr = "NOTICE";
        break;
    case LOG_INFO:
        logLevelstr = "INFO";
        break;
    case LOG_DEBUG:
        logLevelstr = "DEBUG";
        break;
    default:
        logLevelstr = "UNKNOWN";

    }
    snprintf(buf, sizeof buf - 1, "%s [%s] %s %s line:%-5d %s", logLevelstr, sysCategory, fileName, functionName, line, info);
    syslog(LOG_INFO, buf);
    closelog();
}
