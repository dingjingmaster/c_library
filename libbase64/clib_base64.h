#ifndef CLIB_BASE64_H
#define CLIB_BASE64_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t base64_encode(char* target, const void* source, size_t bytes);
size_t base64_encode_url(char* target, const void* source, size_t bytes);
size_t base64_decode(void* target, const char* source, size_t bytes);

size_t base16_encode(char* target, const void* source, size_t bytes);
size_t base16_decode(void* target, const char* source, size_t bytes);


#ifdef __cpluspluc
}
#endif
#endif // CLIB_BASE64_H
