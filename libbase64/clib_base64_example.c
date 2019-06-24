#include <stdio.h>
#include <string.h>
#include "clib_base64.h"

int main() {
    const char* str = "dasda!@#~dsvref4568%^*^7*^&9*&)_+g测试汉字";
    char tar[1024] = {0};
    size_t ret = base64_encode(tar, str, strlen(str));
    printf("encode string length: %ld\n%s\n", ret, tar);

    return 0;
}
