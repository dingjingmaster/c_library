#include <stdio.h>
#include "clib_syslog.h"

int main() {
    syslog_init("clib syslog demo", LOG_LOCAL6);
    CT_SYSLOG(LOG_EMERG, "测试1");
    CT_SYSLOG(LOG_INFO, "测试2");
    CT_SYSLOG(LOG_INFO, "测试3");
    CT_SYSLOG(LOG_INFO, "测试4");
    CT_SYSLOG(LOG_INFO, "测试5");
    CT_SYSLOG(LOG_INFO, "测试6");
    CT_SYSLOG(LOG_INFO, "测试7");
    CT_SYSLOG(LOG_INFO, "测试%s|%d", "fff", 555);
    return 0;
}
