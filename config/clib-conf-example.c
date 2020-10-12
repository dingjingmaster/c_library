/*************************************************************************
> FileName: clib-conf-example.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年10月12日 星期一 12时01分13秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clib-conf.h"
#include "clib-ret.h"

#define BUF_SIZE 10240

int main (int argc, char* argv[])
{
    int ret = 0;
    char* buf = (char*) malloc (BUF_SIZE);

    memset (buf, 0, BUF_SIZE);

    ret = conf_load (buf, "/data/code/c_library/config/re.conf", BUF_SIZE, 6);
    if (ret < DJCLIB_OK) {
        printf ("error:%d\n", (int)ret);
        return -1;
    }

    printf ("log_file = %s\n", conf_value(buf, "log_file", "default"));
    printf ("log_msize = %ld\n", conf_udec(buf, "log_msize", 0));
    printf ("log_knum = %ld\n", conf_udec(buf, "log_knum", 0));

    return 0;
}
