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

#define LOG_FILENAME_LEN            (32)
#define LOG_DIRNAME_LEN             (1024)
#define LOG_PATH_MAX                (2048)
#define PATH_SPLIT                  '/'

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

static log_rotate_t _log_rotate;                                        /* 是否允许分文件 */
static int _log_size = 0;                                               /* 日志文件大小 */
static log_level_t _log_level;                                          /* 输出日至级别 */
static char _log_dir[LOG_DIRNAME_LEN] = "./";                           /* 日志输出文件夹 */
static char _log_prefix[LOG_FILENAME_LEN] = "log";                      /* 日志名称 */
static char _log_suffix[LOG_FILENAME_LEN] = "log";                      /* 日志扩展名 */
static int _log_fd = 0;                                                 /* 当前打开的日志文件描述符 */


static pthread_mutex_t _log_mutex;                                      /* 日志锁 */
static pthread_once_t thread_once = PTHREAD_ONCE_INIT;                  /* 确保初始化一次 */

static int _is_log_init = 0;                                            /* 是否完成初始化 */



//static int _log_fopen(const char* path) {
//    check_dir(path);
//    _log_fp = fopen(path, "a+");
//    if(NULL == _log_fp) {
//        fprintf(stderr, "fopen %s failed: %s\n", path, strerror(errno));
//        fprintf(stderr, "use stderr as output\n");
//        _log_fp = stderr;
//    }
//    return 0;
//}

//static int _log_fclose(void) {
//    return fclose(_log_fp);
//}

//static ssize_t _log_fwrite(struct lovec* vec, int n) {
//    int i = 0, ret = 0;
//    char log_rename[FILENAME_LEN] = {0};
//    unsigned long long tmp_size = get_file_size_by_fp(_log_fp);
//    if(tmp_size > _log_file_size) {
//        if(_log_rotate != 0) {
//            if (EOF == _log_fclose()) {
//                fprintf(stderr, "_log_fclose errno:%d", errno);
//            }
//            _log_fopen_rewrite(_log_name);
//        } else {
//            if (1){}
//            if(EOF == _log_fclose()) {
//                fprintf(stderr, "_log_fclose errno:%d", errno);
//            }
//            log_get_time(_log_name_time, sizeof(_log_name_time), 1);
//            snprintf(log_rename, sizeof(log_rename), "%s%s_%s",
//                     _log_path, _log_name_prefix, _log_name_time);
//            if(-1 == rename(_log_name, log_rename)) {
//                fprintf(stderr, "log file splited %s errno: %d:%s\n",
//                        log_rename, errno, strerror(errno));
//            }
//            _log_fopen(_log_name);
//            if(1){}
//        }
//    }
//    for(i = 0; i < n; ++i) {
//        ret = fprintf(_log_fp, "%s", (char*)vec[i].iov_base);
//        if(ret != (int)vec[i].iov_len) {
//            fprintf(stderr, "fprintf failed: %s\n", strerror(errno));
//            return -1;
//        }
//        if(EOF == fflush(_log_fp)) {
//            fprintf(stderr, "fflush failed: %s\n", strerror(errno));
//            return -1;
//        }
//    }
//    return 0;
//}

//static int _log_open(const char* path) {
//    check_dir(path);
//    _log_fd = open(path, O_RDWR | O_CREAT | O_APPEND, 0644);
//    if(-1 == _log_fd) {
//        fprintf(stderr, "open %s failed: %s\n", path, strerror(errno));
//        fprintf(stderr, "use STDERR_FILEIO as output\n");
//        _log_fd = STDERR_FILENO;
//    }
//    return 0;
//}

//static int _log_close(void) {
//    return close(_log_fd);
//}

//static ssize_t _log_write(struct iovec *vec, int n) {
//    char log_rename[FILENAME_LEN] = {0};
//    unsigned long long tmp_size = get_file_size(_log_name);
//    if (UNLIKELY(tmp_size > _log_file_size)) {
//        if (_log_rotate) {
//            if (-1 == _log_close()) {
//                fprintf(stderr, "_log_close errno:%d", errno);
//            }
//            _log_open_rewrite(_log_name);
//        } else {
//        fprintf(stderr, "%s size= %" PRIu64 " reach max %" PRIu64 ", splited\n",
//                _log_name, (uint64_t)tmp_size, (uint64_t)_log_file_size);
//            if (-1 == _log_close()) {
//                fprintf(stderr, "_log_close errno:%d", errno);
//            }
//            log_get_time(_log_name_time, sizeof(_log_name_time), 1);
//            snprintf(log_rename, sizeof(log_rename), "%s%s_%s",
//                    _log_path, _log_name_prefix, _log_name_time);
//            if (-1 == rename(_log_name, log_rename)) {
//                fprintf(stderr, "log file splited %s error: %d:%s\n",
//                        log_rename, errno , strerror(errno));
//            }
//            _log_open(_log_name);
//            fprintf(stderr, "splited file %s\n", log_rename);
//        }
//    }

//    return writev(_log_fd, vec, n);
//}






//static int _log_print(int level, const char* tag, const char* file, int line, const char* func, const char* msg) {
//    int ret = 0, i = 0;
//    char s_time[LOG_TIME_SIZE] = {0};
//    char s_level[LOG_LEVEL_SIZE] = {0};
//    char s_tag[LOG_TAG_SIZE] = {0};
//    char s_pname[LOG_PNAME_SIZE] = {0};
//    char s_pid[LOG_PNAME_SIZE] = {0};
//    char s_tid[LOG_PNAME_SIZE] = {0};
//    char s_file[LOG_TEXT_SIZE] = {0};
//    char s_msg[LOG_BUF_SIZE] = {0};

//    pthread_mutex_lock(&_log_mutex);
//    _log_get_time(s_time, sizeof(s_time));
//    if(_log_fp == stderr || _log_fd == STDERR_FILENO) {
//        switch(level) {
//        case LOG_EMERG:
//        case LOG_ALERT:
//        case LOG_CRIT:
//        case LOG_ERR:
//            snprintf(s_level, sizeof(s_level), B_RED("%8s"), _log_level_str[level]);
//            snprintf(s_msg, sizeof(s_msg), RED("%s"), msg);
//            break;
//        case LOG_WARNING:
//            snprintf(s_level, sizeof(s_level), B_YELLOW("[%7s]"), _log_level_str[level]);
//            snprintf(s_msg, sizeof(s_msg), YELLOW("%s"), msg);
//            break;
//        case LOG_INFO:
//            snprintf(s_level, sizeof(s_level), B_GREEN("[%7s]"), _log_level_str[level]);
//            snprintf(s_msg, sizeof(s_msg), GREEN("%s"), msg);
//            break;
//        case LOG_DEBUG:
//            snprintf(s_level, sizeof(s_level), B_WHITE("[%7s]"), _log_level_str[level]);
//            snprintf(s_msg, sizeof(s_msg), WHITE("%s"), msg);
//            break;
//        default:
//            snprintf(s_level, sizeof(s_level), "[%7s]", _log_level_str[level]);
//            snprintf(s_msg, sizeof(s_msg), "%s", msg);
//            break;
//        }
//    } else {
//        snprintf(s_level, sizeof(s_level), "[%7s]", _log_level_str[level]);
//        snprintf(s_msg, sizeof(s_msg), "%s", msg);
//    }
//    snprintf(s_pname, sizeof(s_pname), "[%s ", _proc_name);
//    snprintf(s_pid, sizeof(s_pid), "pid:%d ", getpid());
//    snprintf(s_tid, sizeof(s_tid), "tid:%d ", (int)pthread_self());
//    snprintf(s_tag, sizeof(s_tag), "[%s] ", tag);
//    snprintf(s_file, sizeof(s_file), "[%s:%d: %s] ", file, line, func);
//    // 写
////    ret = _log_handle->write()
//    pthread_mutex_unlock(&_log_mutex);
//    return 0;

//}
////

static const char* get_dir(const char* path) {
    char* p = (char*)path + strlen(path);
    for(; p != path; p--) {
        if('/' == *p) {
            *(p + 1) = '\0';
            break;
        }
    }
    return path;
}

static int mkdir_r(const char* path, mode_t mode) {
    int ret = 0;
    char* temp = NULL;
    char* pos = NULL;
    if(NULL == path) {
        return -1;
    }
    temp = strdup(path);
    if(strncmp(temp, "/", 1) == 0) {
        pos += 1;
    } else if(strncmp(temp, "./", 2) == 0) {
        pos += 2;
    }
    for(; *pos != '\0'; ++pos) {
        if(*pos == '/') {
            *pos = '\0';
            if (-1 == (ret = mkdir(temp, mode))) {
                if(errno == EEXIST) {
                    ret = 0;
                } else {
                    fprintf(stderr, "fail to mkdir %s: %d:%s\n",
                            temp, errno, strerror(errno));
                    break;
                }
            }
            *pos = '/';
        }
    }
    if(*(pos - 1) != '/') {
        printf("if %s\n", temp);
        if(-1 == (ret = mkdir(temp, mode))) {
            if(errno == EEXIST) {
                ret = 0;
            } else {
                fprintf(stderr, "failed to mkdir %s: %d:%s\n",
                        temp, errno, strerror(errno));
            }
        }
    }
    free(temp);
    return ret;
}

static int check_dir (const char* path) {
    char* path_tmp = NULL;
    const char* dir = NULL;
    path_tmp = strdup(path);
    if(NULL != strstr(path, "/")) {
        dir = get_dir(path_tmp);
        if (-1 == access(dir, F_OK | W_OK | R_OK)) {
            if(-1 == mkdir_r(dir, 0775)) {
                fprintf(stderr, "mkdir %s failed\n", path_tmp);
                goto RET_ERR;
            }
        }
    }

    free(path_tmp);
    return 0;

RET_ERR:
    free(path_tmp);
    return -1;
}

static void _log_get_time(char* str, int len, int flag) {
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

    if (0 == flag) {
        strftime(date_fmt, 20, "%Y-%m-%d %H:%M:%S", &now_tm);
        snprintf(date_ms, sizeof(date_ms), "%03ld", now_ms);
        snprintf(str, (unsigned long)len, "[%s.%s]", date_fmt, date_ms);
    } else {
        strftime(date_fmt, 20, "%Y_%m_%d_%H_%M_%S", &now_tm);
        snprintf(date_ms, sizeof(date_ms), "%03ld", now_ms);
        snprintf(str, (unsigned long)len, "%s_%s", date_fmt, date_ms);
    }
}

static void log_init_once(void) {
    if(1 == _is_log_init) {
        return;
    }
    _is_log_init = 1;
    pthread_mutex_init(&_log_mutex, NULL);
}

int log_init(log_level_t level, log_rotate_t rotate, int log_size,
             const char *dir, const char *prefix, const char *suffix) {
    if (0 != pthread_once(&thread_once, log_init_once)) {
        fprintf(stderr, "pthread_once error, log_init failed\n");
        return -1;
    }

    if(0 != pthread_mutex_lock(&_log_mutex)) {
        fprintf(stderr, "pthread_mutex_lock error, log_init failed\n");
        return -1;
    }

    _log_level = level;
    _log_rotate = rotate;
    if (log_size > 0) {
        _log_size = log_size;
    } else {
        _log_size = 2 << 20;
    }
    if (NULL != dir) {
        strncpy(_log_dir, dir, sizeof(_log_dir));
    }
    if (NULL != prefix) {
        strncpy(_log_prefix, prefix, sizeof(_log_prefix));
    }
    if (NULL != suffix) {
        strncpy(_log_suffix, suffix, sizeof(_log_suffix));
    }

    // 创建文件夹
    if(0 != check_dir(_log_dir)) {
        fprintf(stderr, "check_dir error, log_init failed\n");
        goto RET_ERR;
    }
    // 打开文件
    char path_name[LOG_PATH_MAX] = {0};
    char time_str[LOG_FILENAME_LEN] = {0};
    _log_get_time(time_str, sizeof(time_str), 1);
    if(0 >= snprintf(path_name, sizeof(path_name), "%s/%s_%s.%s",
                _log_dir, _log_prefix, time_str, _log_suffix)) {
        goto RET_ERR;
    }
    // 打开文件
    _log_fd = open(path_name, O_CREAT | O_RDWR | O_APPEND, 0664);
    if(-1 == _log_fd) {
        fprintf(stderr, "fopen %s error: %s, log_init failed\n",
                path_name, strerror(errno));
        _log_fd = STDERR_FILENO;
        goto RET_ERR;
    }


    if(0 != pthread_mutex_unlock(&_log_mutex)) {
        fprintf(stderr, "pthread_mutex_unlock error, log_init failed\n");
        goto RET_ERR;
    }

    return 0;

RET_ERR:
    pthread_mutex_unlock(&_log_mutex);
    return -1;
}

void log_destroy(void) {
    if(!_is_log_init) {
        return;
    }
    _is_log_init = 0;
    pthread_mutex_destroy(&_log_mutex);
}

//void log_set_level(int level) {
//    if(level > LOG_VERB || level < LOG_EMERG) {
//        _log_level = LOG_LEVEL_DEFAULT;
//    } else {
//        _log_level = level;
//    }
//}

//void log_set_split_size(int size) {
//    if((uint32_t)size > FILESIZE_LEN || size < 0) {
//        _log_file_size = FILESIZE_LEN;
//    } else {
//        _log_file_size = (unsigned long long)size;
//    }
//}

//void log_set_rotate(int enable) {
//    _log_rotate = enable;
//}

//int log_set_path(const char *path) {
//    if ((NULL == path) || (0 == strlen(path))) {
//        return -1;
//    }
//    strncpy(_log_path, path, FILESIZE_LEN - 1);

//    return 0;
//}

//int log_print(int level, const char *tag, const char *file, int line, const char* func, const char* fmt, ...) {
//    va_list ap;
//    char buf[LOG_BUF_SIZE] = {0};
//    int n;
//    if (level > _log_level) {
//        return 0;
//    }
//    if (!_is_log_init) {
//        log_init(0, NULL);
//    }
//    va_start(ap, fmt);
//    n = vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
//    va_end(ap);
//    if(n < 0) {
//        return -1;
//    }

//    return _log_print(level, tag, file, line, func, buf);
//}
