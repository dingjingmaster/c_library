/*************************************************************************
> FileName: gio-samba.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年03月08日 星期一 16时06分29秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>

void ask_question_cb(GMountOperation *op, char *message, char **choices, gpointer a);
void mount_enclosing_volume_callback(GFile *volume, GAsyncResult *res, gpointer a);
void ask_password_cb(GMountOperation *op, const char *message, const char *default_user, const char *default_domain, GAskPasswordFlags flags, gpointer);


/* 匿名登录 */
int main (int argc, char* argv[])
{
    GFile*                      mvolume = NULL;
    GMountOperation*            mop = NULL;
    GCancellable*               mcancel = NULL;

    mcancel = g_cancellable_new();
    mop = g_mount_operation_new();
    mvolume = g_file_new_for_uri ("smb://172.30,50.199/");

    g_mount_operation_set_username(mop, "");
    g_mount_operation_set_password(mop, "");
    g_mount_operation_set_domain(mop, "172.30.50.199");
    g_mount_operation_set_anonymous(mop, TRUE);
    //g_mount_operation_set_password_save(mop, G_PASSWORD_SAVE_FOR_SESSION);

    g_file_mount_enclosing_volume(mvolume, G_MOUNT_MOUNT_NONE, mop, mcancel, mount_enclosing_volume_callback, NULL);

    //g_signal_connect (mop, "aborted", G_CALLBACK (aborted_cb), NULL);
    g_signal_connect (mop, "ask-question", G_CALLBACK(ask_question_cb), NULL);
    g_signal_connect (mop, "ask-password", G_CALLBACK (ask_password_cb), NULL);

    return 0;
}

void mount_enclosing_volume_callback(GFile *volume, GAsyncResult *res, gpointer a)
{
    GError *err = NULL;
    g_file_mount_enclosing_volume_finish (volume, res, &err);

    if (err) {
        printf ("error code:%d, error string:%s\n", err->code, err->message);
        g_error_free (err);
    }
}

void ask_question_cb(GMountOperation *op, char *message, char **choices, gpointer a)
{
    char **choice = choices;
    int i = 0;
    while (*choice) {
        printf ("%\n", *choice);
        g_mount_operation_set_choice(op, i);
        *choice++;
        i++;
    }
    g_mount_operation_reply (op, G_MOUNT_OPERATION_HANDLED);
}

void ask_password_cb(GMountOperation *op, const char *message, const char *default_user, const char *default_domain, GAskPasswordFlags flags, gpointer a)
{
    g_mount_operation_reply (op, G_MOUNT_OPERATION_HANDLED);
}
