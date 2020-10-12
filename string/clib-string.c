/*************************************************************************
> FileName: clib-string.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年10月12日 星期一 19时10分54秒
 ************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "clib-string.h"

char* strzcpy(char *dest, const char *src, size_t n, uint32_t *out_len) 
{
    const char *end;
    char *out;

    out = dest;
    if (n > 0) {
        if (src != NULL) {
            end = src + n - 1;
            while (src < end) {
                if (*src == 0) {
                    break;
                }
                *out++ = *src++;
            }
        }

        /* always output a '\0' terminated string */
        *out++ = 0;
    }
    if (out_len != NULL) {
        *out_len = out - dest;
    }
    return dest;
}

size_t strescape(const char *src, size_t len, char *dst, size_t dst_len) 
{
  int i, j, k;
  char c;
  j = 0;
  for (i = 0; i < len && j < dst_len; ++i) {
    k = 1;
    switch (src[i]) {
        case '\\':
          c = '\\';
          break;
        case '\"':
          c = '\"';
          break;
        case '\b':
          c = 'b';
          break;
        case '\f':
          c = 'f';
          break;
        case '\n':
          c = 'n';
          break;
        case '\r':
          c = 'r';
          break;
        case '\t':
          c = 't';
          break;
        default:
          k = 0;
          c = src[i];
          break;
    }
    if (k > 0 && j + k >= dst_len) {
      break;
    }
    if (k > 0) {
      dst[j++] = '\\';
    }
    dst[j++] = c;
  }

  return j;
}
