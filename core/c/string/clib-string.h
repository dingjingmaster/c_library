/*************************************************************************
> FileName: clib-string.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年10月12日 星期一 19时08分20秒
 ************************************************************************/
#ifndef _CLIB_STRING_H
#define _CLIB_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

char* strzcpy(char *dest, const char *src, size_t n, uint32_t *out_len);
size_t strescape(const char *src, size_t len, char *dst, size_t dst_len);

#ifdef __cplusplus
}
#endif
#endif
