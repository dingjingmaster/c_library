#include "clib-base64.h"
#include <stdint.h>

/**
 *
 * 原始串：1111 1111 1111 1111 1111 1111
 * base64之后：0011 1111 0011 1111 0011 1111 0011 1111
 */

/**
 * 标准 base64 编码
 * 标准编码后，每个字节与字符的对应关系
 * 下标           --- 高2位补0后的值
 * 下标对应字符    --- 要转为的字符
 */
static const char _base64_enc[64] = {
    'A','B','C','D','E','F','G','H','I','J','K','L','M',        /* 0  ~ 12 */
    'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',        /* 13 ~ 25 */
    'a','b','c','d','e','f','g','h','i','j','k','l','m',        /* 26 ~ 38 */
    'n','o','p','q','r','s','t','u','v','w','x','y','z',        /* 39 ~ 51 */
    '0','1','2','3','4','5','6','7','8','9','+','/'             /* 52 ~ 63 */
};

/**
 * base64 解码:
 * 上边编码字符转为 10 进制后 -> 原始数据值
 * 下标           --- base64 编码后一个字符对应的 ascii 十进制值
 * 下标对应值      --- 原始字符高2位补零后的值
 */
static const uint8_t _base64_dec[256] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,             /* 0   ~ 9   */
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,             /* 10  ~ 19  */
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,             /* 20  ~ 29  */
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,             /* 30  ~ 39  */
    0,   0,   0,  62,   0,  62,   0,  63,  52,  53,             /* 40  ~ 49  +(43), -(45), /(47)*/
   54,  55,  56,  57,  58,  59,  60,  61,   0,   0,             /* 50  ~ 59  */
    0,   0,   0,   0,   0,   0,   1,   2,   3,   4,             /* 60  ~ 69  A(65) ~ E(69) */
    5,   6,   7,   8,   9,  10,  11,  12,  13,  14,             /* 70  ~ 79  F(70) ~ O(79) */
   15,  16,  17,  18,  19,  20,  21,  22,  23,  24,             /* 80  ~ 89  P(80) ~ Y(89) */
   25,   0,   0,   0,   0,  63,  00,  26,  27,  28,             /* 90  ~ 99  Z(90); a(97)26 ~ z(122)51 */
   29,  30,  31,  32,  33,  34,  35,  36,  37,  38,             /* 100 ~ 109 */
   39,  40,  41,  42,  43,  44,  45,  46,  47,  48,             /* 110 ~ 119 */
   49,  50,  51,   0,   0,   0,   0,   0,                       /* 120 ~ 127 */
};

size_t _base64_encode(char* target, const void* source, size_t bytes, const char* table) {
    size_t i = 0, j = 0;
    const uint8_t* ptr = (const uint8_t*) source;               /* 设置指针步长为8 */

    /* 原串中能被 3 整除的部分 */
    for(i = j =  0; i < bytes / 3 * 3; i += 3) {
        target[j++] = table[(ptr[i] >> 2) & 0x3F];              /* 取前 6 位bit 高位置零 0x3F == 0x0011 1111 */
        target[j++] = table[((ptr[i] & 0x03) << 4)\
                | ((ptr[i + 1] >> 4) & 0x0F)];                  /* 取第一个 8bit 的后两位 + 第二个 8 bit 的前四位 */
        target[j++] = table[((ptr[i + 1] & 0x0F) << 2)\
                | ((ptr[i + 2] >> 6) & 0x03)];                  /* 取第二个 8bit 的后四位 + 第三个 8bit 的前 两位 */
        target[j++] = table[(ptr[i + 2] & 0x3F)];               /* 取第三个 8bit 的后六位 */
    }

    /* 原串中不能被 3 整除的部分（有一个或者两个） */
    if (i < bytes) {
        target[j++] = table[(ptr[i] >> 2) & 0x3F];              /* 原始三个串中的 第一个 */
        if(i + 1 < bytes) {                                     /* 原始三个中还有 两个 */
            target[j++] = table[((ptr[i] & 0x03) << 4)\
                    | ((ptr[i + 1] >> 4) & 0x0F)];
            target[j++] = table[((ptr[i + 1] & 0x0F) << 2)];
        } else {                                                /* 原始三个中只有 一个 */
            target[j++] = table[((ptr[i] & 0x03) << 4)];
            target[j++] = '=';
        }
        target[j++] = '=';
    }
    return j;
}

size_t base64_encode(char* target, const void* source, size_t bytes) {
    return _base64_encode(target, source, bytes, _base64_enc);
}

size_t base64_decode(void* target, const char* source, size_t bytes) {
    size_t i = 0, j = 0;
    uint8_t* p = (uint8_t*)target;
    const uint8_t* src = (const uint8_t*)source;
    const uint8_t* end = NULL;

    if(0 != bytes % 4) {
        return (unsigned long) - 1;
    }
    end = src + bytes;

    /* 大于 4bit */
    for(j = 0; j < bytes / 4; ++j) {                                                /* 指针由 char 转为 int, 对应数组长度也的改变 */
        p[i++] = (_base64_dec[src[0]] << 2) | (_base64_dec[src[1]] >> 4);           /* 第一个取四位 + 第二个取两位 */
        p[i++] = (_base64_dec[src[1]] << 4) | (_base64_dec[src[2]] >> 2);
        p[i++] = (_base64_dec[src[2]] << 6) | (_base64_dec[src[3]]);
        src += 4;
    }
    /* 等于 4bit */
    if(end == src + 4) {
        p[i++] = (_base64_dec[src[0]] << 2) | (_base64_dec[src[1]] >> 4);
        if('=' != src[2]) {
            p[i++] = (_base64_dec[src[1]] << 4) | (_base64_dec[src[2]] >> 2);
        }
        if('=' != src[3]) {
            p[i++] = (_base64_dec[src[2]] << 6) | (_base64_dec[src[3]]);
        }
    }
    return i;
}
