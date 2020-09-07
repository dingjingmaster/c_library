#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "clib-base64.h"

int main() {
    const char* str = "dasda!@#~dsvref4568%^*^7*^&9*&)_+g测试汉字";
    char tar[1024] = {0};
    char test[1024] = {0};

    size_t ret = base64_encode(tar, str, strlen(str));
    printf("encode string length: %ld\n%s\n", ret, tar);

    ret = base64_decode(test, tar, strlen(tar));
    printf("encode string length: %ld\n%s\n", ret, test);

    return 0;
}
