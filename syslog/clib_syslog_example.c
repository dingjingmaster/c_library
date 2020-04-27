#include <stdio.h>

#include "clib_syslog.h"

int main() {
    syslog_init("clib syslog demo", LOG_DEBUG, LOG_LOCAL6);
    CT_SYSLOG(LOG_EMERG, "测试1");
    CT_SYSLOG(LOG_ALERT, "测试2");
    CT_SYSLOG(LOG_CRIT, "测试3");
    CT_SYSLOG(LOG_ERR, "测试4");
    CT_SYSLOG(LOG_WARNING, "测试5");
    CT_SYSLOG(LOG_NOTICE, "测试6");
    CT_SYSLOG(LOG_INFO, "测试7");
    CT_SYSLOG(LOG_DEBUG, "测试%s|%d", "fff", 555);
    return 0;
}
