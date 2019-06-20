#include "clib_log.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/param.h>
#include <sys/types.h>

#define LOG_IOVEC_MAX               (10)
#define FILENAME_LEN                (256)                               /* log 路径名字最大长度 */
#define FILESIZE_LEN                (10*1024*1024ul)
#define LOG_BUF_SIZE                (1024)
#define LOG_TIME_SIZE               (32)
#define LOG_LEVEL_SIZE              (32)
#define LOG_TAG_SIZE                (32)
#define LOG_PNAME_SIZE              (32)
#define LOG_TEXT_SIZE               (256)
#define LOG_LEVEL_DEFAULT           LOG_INFO


#define NAME_MAX                    255
#define PATH_SPLIT                  '/'

/* 颜色 -- 开始 */
#if defined (__WIN32__) || defined (WIN32) || defined (_MSC_VER)
#define B_RED(str)                  str
#define B_GREEN(str)                str
#define B_YELLOW(str)               str
#define B_BLUE(str)                 str
#define B_MAGENTA(str)              str
#define B_CYAN(str)                 str
#define B_WHITE(str)                str
#define RED(str)                    str
#define GREEN(str)                  str
#define YELLOW(str)                 str
#define BLUE(str)                   str
#define MAGENTA(str)                str
#define CYAN(str)                   str
#define WHITE(str)                  str
#else
#define FG_BLACK                    30
#define FG_RED                      31
#define FG_GREEN                    32
#define FG_YELLOW                   33
#define FG_BLUE                     34
#define FG_MAGENTA                  35
#define FG_CYAN                     36
#define FG_WHITE                    37
#define BG_BLACK                    40
#define BG_RED                      41
#define BG_GREEN                    42
#define BG_YELLOW                   43
#define BG_BLUE                     44
#define BG_MAGENTA                  45
#define BG_CYAN                     46
#define BG_WHITE                    47
#define B_RED(str)                  "\033[1;31m" str "\033[0m"
#define B_GREEN(str)                "\033[1;32m" str "\033[0m"
#define B_YELLOW(str)               "\033[1;33m" str "\033[0m"
#define B_BLUE(str)                 "\033[1;34m" str "\033[0m"
#define B_MAGENTA(str)              "\033[1;35m" str "\033[0m"
#define B_CYAN(str)                 "\033[1;36m" str "\033[0m"
#define B_WHITE(str)                "\033[1;37m" str "\033[0m"
#define RED(str)                    "\033[31m" str "\033[0m"
#define GREEN(str)                  "\033[32m" str "\033[0m"
#define YELLOW(str)                 "\033[33m" str "\033[0m"
#define BLUE(str)                   "\033[34m" str "\033[0m"
#define MAGENTA(str)                "\033[35m" str "\033[0m"
#define CYAN(str)                   "\033[36m" str "\033[0m"
#define WHITE(str)                  "\033[37m" str "\033[0m"
#endif
/* 颜色 -- 结束 */

static const char* _log_level_str[] = {
    "EMERG",
    "ALERT",
    "CRIT",
    "ERR",
    "WARN",
    "NOTICE",
    "INFO",
    "DEBUG",
    "VERBOSE",
    NULL
};

// begin
typedef struct log_ops {
    int (*open)(const char* path);
    ssize_t (*write)(struct iovec* vec, int n);
    int (*close)(void);
} log_ops_t;
static log_ops_t log_fio_ops = {
//    _log_fopen,
//    _log_fwrite,
//    _log_fclose,
};
static log_ops_t log_io_ops = {
//    _log_open,
//    _log_write,
//    _log_close,
};
static log_ops_t* _log_handle = NULL;                                   /* log具体输出 */
// end

static int _is_log_init = 0;                                            /* 是否完成初始化 */
static int _log_fd = 0;
static FILE* _log_fp = NULL;
static int _log_level = LOG_LEVEL_DEFAULT;
static int _log_syslog = 0;
static char _log_path[FILENAME_LEN] = {0};                              /* log 存储路径 */
static char _log_name[FILENAME_LEN];
static char _log_name_prefix[FILENAME_LEN];
static char _log_name_time[FILENAME_LEN];
static pthread_mutex_t _log_mutex;                                      /* 日志锁 */
static int _log_prefix = 0;
static int _log_output = 0;
static int _log_use_io = 0;                                             /* 是否使用默认输出(否则是文件输出) */
static char _proc_name[NAME_MAX];
static unsigned long long _log_file_size = FILESIZE_LEN;
static int _log_type = LOG_OUTPUT_CONSOLE;                              /* 输出类型(默认控制台) */
static const char *_log_ident;

static int _log_rotate = 0;


static pthread_once_t thread_once = PTHREAD_ONCE_INIT;                  /* 确保初始化一次 */

//
static void _log_get_time(char* str, int len) {
    char date_fmt[20] = {0};
    char date_ms[4] = {0};
    struct timeval tv;
    struct tm now_tm;
    long now_ms;
    time_t now_sec;
    gettimeofday(&tv, NULL);
    now_sec = tv.tv_sec;
    now_ms = tv.tv_usec/1000;
    localtime_r(&now_sec, &now_tm);
    strftime(date_fmt, 20, "%Y-%m-%d %H:%M:%S", &now_tm);
    snprintf(date_ms, sizeof(date_ms), "%03ld", now_ms);
    snprintf(str, (unsigned long)len, "[%s.%s]", date_fmt, date_ms);
}
static void log_init_once(void) {
    int type = _log_type;
    const char* ident = _log_ident;
    if(_is_log_init) {
        return;
    }
    _log_handle = &log_fio_ops;

    _is_log_init = 1;
    pthread_mutex_init(&_log_mutex, NULL);
}

static int _log_print(int level, const char* tag, const char* file, int line, const char* func, const char* msg) {
    int ret = 0, i = 0;
    char s_time[LOG_TIME_SIZE] = {0};
    char s_level[LOG_LEVEL_SIZE] = {0};
    char s_tag[LOG_TAG_SIZE] = {0};
    char s_pname[LOG_PNAME_SIZE] = {0};
    char s_pid[LOG_PNAME_SIZE] = {0};
    char s_tid[LOG_PNAME_SIZE] = {0};
    char s_file[LOG_TEXT_SIZE] = {0};
    char s_msg[LOG_BUF_SIZE] = {0};

    pthread_mutex_lock(&_log_mutex);
    _log_get_time(s_time, sizeof(s_time));
    if(_log_fp == stderr || _log_fd == STDERR_FILENO) {
        switch(level) {
        case LOG_EMERG:
        case LOG_ALERT:
        case LOG_CRIT:
        case LOG_ERR:
            snprintf(s_level, sizeof(s_level), B_RED("%8s"), _log_level_str[level]);
            snprintf(s_msg, sizeof(s_msg), RED("%s"), msg);
            break;
        case LOG_WARNING:
            snprintf(s_level, sizeof(s_level), B_YELLOW("[%7s]"), _log_level_str[level]);
            snprintf(s_msg, sizeof(s_msg), YELLOW("%s"), msg);
            break;
        case LOG_INFO:
            snprintf(s_level, sizeof(s_level), B_GREEN("[%7s]"), _log_level_str[level]);
            snprintf(s_msg, sizeof(s_msg), GREEN("%s"), msg);
            break;
        case LOG_DEBUG:
            snprintf(s_level, sizeof(s_level), B_WHITE("[%7s]"), _log_level_str[level]);
            snprintf(s_msg, sizeof(s_msg), WHITE("%s"), msg);
            break;
        default:
            snprintf(s_level, sizeof(s_level), "[%7s]", _log_level_str[level]);
            snprintf(s_msg, sizeof(s_msg), "%s", msg);
            break;
        }
    } else {
        snprintf(s_level, sizeof(s_level), "[%7s]", _log_level_str[level]);
        snprintf(s_msg, sizeof(s_msg), "%s", msg);
    }
    snprintf(s_pname, sizeof(s_pname), "[%s ", _proc_name);
    snprintf(s_pid, sizeof(s_pid), "pid:%d ", getpid());
    snprintf(s_tid, sizeof(s_tid), "tid:%d ", (int)pthread_self());
    snprintf(s_tag, sizeof(s_tag), "[%s] ", tag);
    snprintf(s_file, sizeof(s_file), "[%s:%d: %s] ", file, line, func);
    // 写
//    ret = _log_handle->write()
    pthread_mutex_unlock(&_log_mutex);
    return 0;

}
//

/* log 初始化 */
int log_init(int type, const char *ident) {
    _log_type = type;
    _log_ident = ident;
    if (0 != pthread_once(&thread_once, log_init_once)) {
        fprintf(stderr, "pthread_once failed\n");
    }

    return 0;
}

void log_destroy(void) {
    if(!_is_log_init) {
        return;
    }
    _is_log_init = 0;
    pthread_mutex_destroy(&_log_mutex);
}

void log_set_level(int level) {
    if(level > LOG_VERB || level < LOG_EMERG) {
        _log_level = LOG_LEVEL_DEFAULT;
    } else {
        _log_level = level;
    }
}

void log_set_split_size(int size) {
    if((uint32_t)size > FILESIZE_LEN || size < 0) {
        _log_file_size = FILESIZE_LEN;
    } else {
        _log_file_size = (unsigned long long)size;
    }
}

void log_set_rotate(int enable) {
    _log_rotate = enable;
}

int log_set_path(const char *path) {
    if ((NULL == path) || (0 == strlen(path))) {
        return -1;
    }
    strncpy(_log_path, path, FILESIZE_LEN - 1);

    return 0;
}

int log_print(int level, const char *tag, const char *file, int line, const char* func, const char* fmt, ...) {
    va_list ap;
    char buf[LOG_BUF_SIZE] = {0};
    int n;
    if (level > _log_level) {
        return 0;
    }
    if (!_is_log_init) {
        log_init(0, NULL);
    }
    va_start(ap, fmt);
    n = vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
    va_end(ap);
    if(n < 0) {
        return -1;
    }

    return _log_print(level, tag, file, line, func, buf);
}
