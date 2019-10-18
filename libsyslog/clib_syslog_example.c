#include <stdio.h>
#include "clib_syslog.h"

int main() {
    syslog_init("clib syslog demo", LOG_LOCAL6);
    CT_SYSLOG(LOG_INFO, "测试1");
    printf("Hello World!\n");
    return 0;
}
