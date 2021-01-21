/*************************************************************************
> FileName: demo1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年01月20日 星期三 20时19分44秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>

#define SYSTEM_PROXY        "org.gnome.system.proxy"
#define SYSTEM_PROXY_HTTP   "org.gnome.system.proxy.http"
#define SYSTEM_PROXY_HTTPS  "org.gnome.system.proxy.https"

void change_func        (GSettings* gs, char* key, gpointer udata);
void change_func_http   (GSettings* gs, char* key, gpointer udata);
void change_func_https  (GSettings* gs, char* key, gpointer udata);

int main (int argc, char* argv[])
{
    gtk_init (&argc, &argv);

    GSettings* gsetting = g_settings_new (SYSTEM_PROXY);
    GSettings* gsetting_http = g_settings_new (SYSTEM_PROXY_HTTP);
    GSettings* gsetting_https = g_settings_new (SYSTEM_PROXY_HTTPS);
    if (NULL == gsetting) {
        printf ("错误!\n");
        return -1;
    }

    g_signal_connect (gsetting, "changed", change_func, NULL);
    g_signal_connect (gsetting_http, "changed", change_func, NULL);
    g_signal_connect (gsetting_https, "changed", change_func, NULL);

    gtk_main();

    return 0;
}

void change_func (GSettings* gs, char* key, gpointer udata)
{
    GVariant* val = g_settings_get_value (gs, key);
    GVariantType* type = g_variant_get_type (val);

    printf ("changed --- key:%s -- value:%s\n", key, g_variant_get_string (val, NULL));
}

