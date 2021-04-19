/*************************************************************************
> FileName: clib-global-test.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年04月10日 星期六 10时33分19秒
 ************************************************************************/
#include <stdio.h>
#include "core/clib-global.h"

int main (int argc, char* argv[])
{
    printf ("version: %d\n", CLIB_VERSION_CHECK(1, 2, 3));
    printf ("cint8    -- 8  bits: %s  -- %d bytes\n", sizeof(cint8)   * 8 == 8  ? "true" : "false", sizeof (cint8));
    printf ("cuint8   -- 8  bits: %s  -- %d bytes\n", sizeof(cuint8)  * 8 == 8  ? "true" : "false", sizeof (cuint8));
    printf ("cint16   -- 16 bits: %s  -- %d bytes\n", sizeof(cint16)  * 8 == 16 ? "true" : "false", sizeof (cint16));
    printf ("cuint16  -- 16 bits: %s  -- %d bytes\n", sizeof(cuint16) * 8 == 16 ? "true" : "false", sizeof (cuint16));
    printf ("cint32   -- 32 bits: %s  -- %d bytes\n", sizeof(cint32)  * 8 == 32 ? "true" : "false", sizeof (cint32));
    printf ("cuint32  -- 32 bits: %s  -- %d bytes\n", sizeof(cuint32) * 8 == 32 ? "true" : "false", sizeof (cuint32));
    printf ("cint64   -- 64 bits: %s  -- %d bytes\n", sizeof(cint64)  * 8 == 64 ? "true" : "false", sizeof (cint64));
    printf ("cuint64  -- 64 bits: %s  -- %d bytes\n", sizeof(cuint64) * 8 == 64 ? "true" : "false", sizeof (cuint64));

    return 0;
}
