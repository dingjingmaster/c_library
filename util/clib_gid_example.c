/*************************************************************************
> FileName: clib_gid_example.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月06日 星期五 10时16分12秒
 ************************************************************************/
#include "clib_util.h"

int main () 
{
    int uid = 0;
    int gid = 0;
    CLIB_RET ret;

    ret = system_gid_byname ("dingjing", &uid, &gid, NULL, 0, NULL, 0);

    printf ("ret:%d, uid:%d, gid:%d\n", ret, uid, gid);
    
    return 0;
}
