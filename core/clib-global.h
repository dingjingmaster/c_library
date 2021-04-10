/*************************************************************************
> FileName: clib-global.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年04月10日 星期六 10时27分46秒
 ************************************************************************/
#ifndef _CLIB_GLOBAL_H
#define _CLIB_GLOBAL_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>

#define CLIB_VERSION_MAJOR                                  (0)
#define CLIB_VERSION_MINOR                                  (0)
#define CLIB_VERSION_PATCH                                  (0)

#define CLIB_VERSION_CHECK(major, minor, patch)             ((major<<16)|(minor<<8)|(patch))
#define CLIB_VERSION                                        CLIB_VERSION_CHECK(CLIB_VERSION_MAJOR, CLIB_VERSION_MINOR, CLIB_VERSION_PATCH)

typedef signed char                     cint8;              // 8 bit signed
typedef unsigned char                   cuint8;             // 8 bit unsigned
typedef short                           cint16;             // 16 bit signed
typedef unsigned short                  cuint16;            // 16 bit unsigned
typedef int                             cint32;             // 32 bit signed
typedef unsigned int                    cuint32;            // 32 bit unsogned
typedef long long                       cint64;             // 64 bit signed
typedef unsigned long long              cuint64;            // 64 bit unsigned


#ifdef __cplusplus
}
#endif
#endif
