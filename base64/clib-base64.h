#ifndef CLIBBASE64_H
#define CLIBBASE64_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 标准 base64 编码
 *
 * @param target: 编码结果
 * @param source: 要编码的字符串长度
 * @param bytes: 要编码的字符串长度
 *
 * @param return: 返回编码后的长度
 */
size_t base64_encode(char* target, const void* source, size_t bytes);
size_t base64_decode(void* target, const char* source, size_t bytes);

size_t base16_encode(char* target, const void* source, size_t bytes);
size_t base16_decode(void* target, const char* source, size_t bytes);


#ifdef __cpluspluc
}
#endif
#endif // CLIB_BASE64_H
