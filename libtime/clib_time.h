#ifndef CLIB_TIME_H
#define CLIB_TIME_H
#include <time.h>
#include <stdint.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _time_info_t {
    uint64_t        utc_msec;
    uint32_t        utc;
    uint8_t         mon;
    uint8_t         day;
    uint8_t         hour;
    uint8_t         min;
    uint8_t         sec;
    uint16_t        msec;
    int8_t          timezone;
    char            str[32];
} time_info_t;

/* 精确到 秒 */
uint64_t time_get_sec();
char* time_get_sec_str();
char* time_get_str_human(char* str, int len);
char* time_get_str_human_by_utc(uint32_t utc, char* str, int len);
char* time_get_str_human_by_msec(uint64_t msec, char* str, int len);
char* time_get_str_human_by_timeval(struct timeval* val, char* str, int len);


#ifdef __cplusplus
}
#endif
#endif // CLIB_TIME_H
