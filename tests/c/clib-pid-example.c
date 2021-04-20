/*************************************************************************
> FileName: ./clib-pid-example.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月06日 星期五 11时41分11秒
 ************************************************************************/
#include "../../core/c/util/clib-util.h"

int main () 
{
    char* pname = "";
    char* uname = "dingjing";
    int pid = 0;
    int ppid = 0;

    system_pid_byupname (pname, uname, &pid, &ppid);

    return 0;
}
