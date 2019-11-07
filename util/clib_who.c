/*************************************************************************
> FileName: clib_who.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年21月07日 星期四 10时00分10秒
 ************************************************************************/
#include "clib_who.h"
#include <utmp.h>
#include <stdlib.h>
#include <string.h>

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

enum bool {
    true,
    false,
};

static enum bool who_array_exist(char** arr, char* user, int size);
static enum bool who_array_append(char*** arr, char* ele, int* index);
static void who_array_free(char*** arr, int size);

int who_user_num () {
    int user = 0;
    struct utmp* u;

    char** set = (char**)malloc(sizeof(char*) * MAX_USER);
    if (NULL == set) {
        goto end;
    }

    while (NULL != (u = getutent())) {
        if (EMPTY == u->ut_type) {
            continue;
        }
        if (USER_PROCESS == u->ut_type) {
            if (false == who_array_append(&set, u->ut_user, &user)) {
                goto end;
            }
        }
    }

    return user;
end:
    endutent();
    who_array_free(&set, user);

    return -1;
}

static enum bool who_array_append (char*** arr, char* ele, int* index) {
    char** array = *arr;
    char** array_tmp = NULL;
    static int tim = 1;

    if (*index >= MAX_USER * tim) {
        ++tim;
        array_tmp = (char**)malloc(sizeof(char*) * MAX_USER * tim);
        if (NULL == array_tmp) {
            goto end;
        }
        array_tmp = memcpy(*array_tmp, *array, MAX_USER * (tim - 1));
        free(array);
        array = array_tmp;
    }
    if (false == who_array_exist(array, ele, *index)) {
        int name_len = strlen(ele) + 1;
        char* name = (char*) malloc (sizeof (char) * name_len);
        memset(name, 0, name_len);
        strncpy(name, ele, name_len - 1);
        array[*index] = name;
        *index += 1;
    }
    return true;
end:
    who_array_free (arr, *index);
    free(array);
    free(array_tmp);
    return false;
}

static enum bool who_array_exist(char** arr, char* user, int size) {
    int i = 0;
    int unl = 0;
    enum bool exist = false;
    int ul = strlen(user);
    for (i = 0; i < size; ++i) {
        unl = strlen(arr[i]);
        if ((unl == ul) && (0 == strncmp(arr[i], user, unl)) ) {
            exist = true;
        }
    }

    return exist;
}

static void who_array_free(char*** arr, int size) {
    if (NULL == *arr) {
        return;
    }
    char** array = *arr;
    int i = 0;

    for (i = 0; i < size; ++i) {
        free (array[i]);
    }
    free (array);
    *arr = NULL;
}
