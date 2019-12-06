/*************************************************************************
> FileName: clib_util.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年21月07日 星期四 10时00分10秒
 ************************************************************************/
#include "clib_util.h"
#include <utmp.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <unistd.h>

/**
 * ut_type的值：
 *  EMPTY：此为空的记录.
 *  RUN_LVL：记录系统run－level 的改变
 *  BOOT_TIME：记录系统开机时间
 *  NEW_TIME：记录系统时间改变后的时间
 *  OLD_TINE：记录当改变系统时间时的时间.
 *  INIT_PROCESS：记录一个由init 衍生出来的进程.
 *  LOGIN_PROCESS：记录 login 进程.
 *  USER_PROCESS：记录一般进程.
 *  DEAD_PROCESS：记录一结束的进程.
 *  ACCOUNTING：目前尚未使用.
 */

int MAX_USER = 100;

/* 公共函数 -- 开始 */
static CLIB_RET common_read_line (int fd, char* buf, int buf_size);
static CLIB_RET common_str_is_number (const char* str);
/* 公共函数 -- 结束 */

/* 用户数量获取 -- 开始 */
static CLIB_RET who_array_exist(char** arr, char* user, int size);
static CLIB_RET who_array_append(char*** arr, char* user, char* host, int* index);
static void who_array_free(char*** arr, int size);
/* 用户数量获取 -- 结束 */

CLIB_RET system_user_num (int* user)
{
    if (NULL == user) {
        return RET_ERROR;
    }

    struct utmp* u;
    *user = 0;

    char** set = (char**)malloc(sizeof(char*) * MAX_USER);
    if (NULL == set)
        goto end;

    setutent();
    while (NULL != (u = getutent())) {
        if (EMPTY == u->ut_type) {
            continue;
        }
        if (USER_PROCESS == u->ut_type) {
            if (RET_FALSE == who_array_append(&set, u->ut_user, u->ut_host, user))
                goto end;
        }
    }

    endutent();
    who_array_free(&set, *user);
    return RET_OK;

end:
    endutent();
    who_array_free(&set, *user);

    return RET_ERROR;
}


CLIB_RET system_gid_byname (const char* name, int* uid, int* gid,
        char* long_name, int long_name_len, char* home, int home_len) 
{
    if (NULL == name || uid == NULL) {
        return RET_ERROR;
    }

    int i = 0;
    int j = 0;
    int fd = 0;
    int split = 0;
    int name_len = 0;
    char buf[1024] = {0};
    char line_buf[1024] = {0};
    CLIB_RET ret;

    fd = open (CLIB_SYSTEM_ETC_PASSWD, O_RDONLY);
    if (-1 == fd) {
        return RET_ERROR;
    }

    name_len = strlen (name);
    
    while (1) {
        memset (buf, 0, sizeof buf);
        ret = common_read_line (fd, line_buf, sizeof line_buf);
        if (RET_TRUE != ret) return RET_ERROR;
        for (i = 0, j = 0; i < sizeof line_buf; ++i) {
            if ((split == 0) && (':' == line_buf[i])) {
                if (0 == strncmp(buf, name, name_len)) {
                    j = 0;
                    split = 1;
                    memset (buf, 0, sizeof buf);
                } else {
                    break;
                }
            } else if ((split == 1) && (':' == line_buf[i])) {
                j = 0;
                split = 2;
                memset (buf, 0, sizeof buf);
            } else if ((split == 2) && (':' == line_buf[i])) {
                j = 0;
                split = 3;
                *uid = atoi (buf);
                memset (buf, 0, sizeof buf);
            } else if ((split == 3) && (':' == line_buf[i])) {
                j = 0;
                split = 4;
                if (NULL != gid) *gid = atoi (buf);
                memset (buf, 0, sizeof buf);
            } else if ((split == 4) && (':' == line_buf[i])) {
                j = 0;
                split = 5;
                if (NULL != long_name) {
                    if (long_name_len < strlen(buf))
                        return RET_NOTENOUGHMEM;
                    strncpy (long_name, buf, long_name_len - 1);
                }
                memset (buf, 0, sizeof buf);
            } else if ((split == 5) && (':' == line_buf[i])) {
                j = 0;
                split = 6;
                if (NULL != home) {
                    if (home_len < strlen(buf))
                        return RET_NOTENOUGHMEM;
                    strncpy (home, buf, long_name_len - 1);
                }
                memset (buf, 0, sizeof buf);
            } else {
                buf[j++] = line_buf[i];
            }
        }
        if (split > 0) break;
    }
    close (fd);
    return RET_OK;
}


CLIB_RET system_pid_byupname (const char* pname, const char* uname, int* pid, int* ppid) 
{
    if (NULL == pname || NULL == uname || NULL == pid || NULL == ppid) {
        return RET_ERROR;
    }
    
    int uid = 0;
    CLIB_RET ret = RET_ERROR;
    DIR* dir = NULL;
    struct dirent* ptr;
    char path[1024] = {0};

    ret = system_gid_byname (uname, &uid, NULL, NULL, 0, NULL, 0);
    if (RET_OK != ret) {
        return RET_ERROR;
    }

    dir = opendir (CLIB_SYSTEM_PROC);
    if (NULL == dir) {
        return RET_ERROR;
    }

    while (NULL != (ptr = readdir(dir))) {
        if (0 == strcmp(ptr->d_name, ".") || 0 == strcmp(ptr->d_name, "..")) {
            continue;
        } else if (4 == ptr->d_type) {                          // 文件夹
            if (RET_OK == common_str_is_number(ptr->d_name)) {
                memset (path, 0, sizeof path);
                snprintf (path, sizeof path - 1, "%s%s/status", CLIB_SYSTEM_PROC, ptr->d_name);
                // 打开文件并读取
                int fd = open (path, O_RDONLY);
                if (fd >= 0) {
                    printf ("%s\n", path);
                }
            }
        }
    }

    closedir (dir);

    return RET_OK;
}


/* 一些公共函数 */
static CLIB_RET common_str_is_number (const char* str)
{
    if (NULL == str) {
        return RET_ERROR;
    }

    int i = 0;
    int strnum = strlen (str);

    for (i = 0; i < strnum; ++i) {
        if (str[i] < 48 || str[i] > 57) {
            return RET_ERROR;
        }
    }

    return RET_OK;
}

static CLIB_RET common_read_line (int fd, char* buf, int buf_size)
{
    char tmp = '\0';
    int i = 0;

    memset (buf, 0, buf_size);
    for (i = 0; i < buf_size - 1; ++i) {
        int ret = read (fd, &tmp, 1);
        if (ret < 0) return RET_ERROR;
        if ((EOF == tmp) || ('\n' == tmp) || ('\r' == tmp)) return RET_TRUE;
        if (('\n' != tmp) || ('\r' != tmp)) buf[i] = tmp;
    }

    return RET_NOTENOUGHMEM;
}

static CLIB_RET who_array_append (char*** arr, char* user, char* host, int* index)
{
    int i = 0;
    int j = 0;
    char** array = *arr;
    char** array_tmp = NULL;
    static int tim = 1;
    int hostlen = 0;
    char hostbuf[64] = {0};

    hostlen = strlen(host);
    if (hostlen <= 1) {
        return RET_FALSE;
    }
    if (*host == ':') {
        for (i = 1; i < hostlen; ++i) {
            if (*(host + i) == '.') {
                break;
            } else {
                hostbuf[j] = *(host + i);
                ++j;
            }
        }
    } else {
        snprintf(hostbuf, sizeof hostbuf, "%s", host);
    }

    int name_len = strlen(user) + strlen(hostbuf) + 6;

    char* name = (char*) malloc (sizeof (char) * name_len);
    memset(name, 0, name_len);
    snprintf(name, name_len, "%s(%s)", user, hostbuf);

    if (*index >= MAX_USER * tim) {
        ++tim;
        array_tmp = (char**)malloc(sizeof(char*) * MAX_USER * tim);
        if (NULL == array_tmp) 
            goto end;
        array_tmp = memcpy(*array_tmp, *array, MAX_USER * (tim - 1));
        free(array);
        array = array_tmp;
    }
    if (RET_FALSE == who_array_exist(array, name, *index)) {
        array[*index] = name;
        *index += 1;
    }

    return RET_TRUE;
end:
    who_array_free (arr, *index);
    free(array);
    free(array_tmp);
    return RET_FALSE;
}

static CLIB_RET who_array_exist(char** arr, char* user, int size)
{
    int i = 0;
    int unl = 0;
    enum bool exist = RET_FALSE;
    int ul = strlen(user);
    for (i = 0; i < size; ++i) {
        unl = strlen(arr[i]);
        if ((unl == ul) && (0 == strncmp(arr[i], user, unl)))
            exist = RET_TRUE;
    }

    return exist;
}

static void who_array_free(char*** arr, int size)
{
    if (NULL == *arr) 
        return;
    char** array = *arr;
    int i = 0;

    for (i = 0; i < size; ++i)
        free (array[i]);
    free (array);
    *arr = NULL;
}


