/*************************************************************************
> FileName: clib_util.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年11月07日 星期四 21时00分07秒
 ************************************************************************/
#ifndef _CLIB_UTIL_H
#define _CLIB_UTIL_H
#include "clib_ret.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * 返回登录的用户数量
 * @param user: 当前用户数量(输出参数)
 * @return: 
 *      成功: RET_OK，
 *      失败:
 *          RET_ERROR 错误
 */
CLIB_RET system_user_num (int* user);

/**
 * 根据用户名返回用户id
 * @param name: 传入要查找的用户名
 * @param uid: 用户id (输出参数)
 * @param gid: 组id (输出参数)
 * @param long_name: 全名 (输出参数)
 * @param long_name_len: 存户全名的长度
 * @param home: 用户家目录 (输出参数)
 * @param home_len: 存储家目录的长度
 * @return:
 *      成功: RET_OK
 *      失败:
 *          其它错误: RET_ERROR
 *          分配内存不够: RET_NOTENOUGHMEM
 */
CLIB_RET system_gid_byname (const char* name, int* uid, int* gid,
        char* long_name, int long_name_len, char* home, int home_len);

/**
 * 根据用户名和进程名返回进程id
 * @param pname: 进程名字
 * @param uname: 用户名字
 * @param pid: 进程id(输出参数)
 * @param ppid: 父进程id(输出参数)
 * @return:
 *      成功: RET_OK
 *      失败: RET_ERROR
 */
CLIB_RET system_pid_byupname (const char* pname, const char* uname,
        int* pid, int* ppid);


#endif
