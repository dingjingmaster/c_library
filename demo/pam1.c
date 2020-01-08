#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>

/* 回调函数 */
static int login_conv(int num_msg, const struct pam_message **msg, struct pam_response **response, void *appdata_ptr);
pam_handle_t *pamh;  // 进行认证的PAM句柄

/* 出错则清理现场并退出 */
static void login_exit(int exit_code)
{
    if (pamh)
        pam_end(pamh, PAM_ABORT);
    _exit(exit_code);
}

int main(int argc, char *argv[], char **renvp)
{
        passwd *pwd;
        int status = -1;
        const char* ttyn,*p,*service;
        const char * user = argv[1];
        struct pam_conv pam_conv = {login_conv, argv[2]};

    if (argc != 4)
        {
                printf("please inpute username and passwd\n");
                _exit(0);
        }
    /* 初始化，并提供一个回调函数 */
    status = pam_start(argv[3], user, &pam_conv, &pamh);
   
    pam_get_item(pamh ,PAM_SERVICE ,(const void **)&service);
    printf("Service is %s\n",service);
        
    if (status != PAM_SUCCESS)
    {   
            fprintf(stderr,"start error: %s\n", pam_strerror(pamh, status)); /* 显示错误原因 */
        login_exit(1);
        }
        pam_get_item(pamh,PAM_USER,(const void **)&p);/*验证用户是否存在*/
        if (getpwnam(p) == NULL)
        {
                printf("user does not exist\n");
                _exit(0);
        }
    status = pam_authenticate(pamh, 0);        /* 密码认证管理，检查用户输入的密码是否正确 */
    if (status != PAM_SUCCESS)
    {
        fprintf(stderr,"authenticate error: %s\n", pam_strerror(pamh, status)); /* 显示错误原因 */
        login_exit(1);
    }
        else
                printf("authenticate success!\n");
    getchar();
    pam_end(pamh, PAM_SUCCESS);  /* PAM事务的结束 */

}

/* 这个回调函数被PAM认证模块调用以便显示错误信息或者或者用来取得用户输入，采用图形界面的服务程序则应使用图形界面来取得 用户输入或显示提示信息*/
int login_conv(int num_msg, const struct pam_message **msg,
         struct pam_response **response, void *appdata_ptr)
{
    struct pam_message  *m = (struct pam_message *)*msg;
           struct pam_response *r ;
           while (num_msg--)
    {
        switch (m-&gt;msg_style)
        {
            case PAM_PROMPT_ECHO_OFF:
                    r               = (struct pam_response *)malloc(sizeof(struct pam_response));
                            r-&gt;resp         = (char *)malloc(strlen((char*)appdata_ptr));
                            r-&gt;resp_retcode = 0;
                strcpy(r-&gt;resp,(char*)appdata_ptr);
                *response = r;
                break;
            case PAM_PROMPT_ECHO_ON:
                (void) fputs(m-&gt;msg, stdout);
                r-&gt;resp = (char*)malloc(PAM_MAX_RESP_SIZE);
                fgets(r-&gt;resp, PAM_MAX_RESP_SIZE, stdin);
                /* add code here to remove \n from fputs */
                break;
            case PAM_ERROR_MSG:
                (void) fputs(m-&gt;msg, stderr);
                break;
            case PAM_TEXT_INFO:
                (void) fputs(m-&gt;msg, stdout);
                break;
        }
    }
    return (PAM_SUCCESS);
} 
