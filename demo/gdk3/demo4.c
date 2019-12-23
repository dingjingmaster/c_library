/*************************************************************************
> FileName: demo4.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月23日 星期一 18时20分35秒
 ************************************************************************/
#include <gdk/gdk.h>

int main (int argc, char* argv[])
{
    GError* error = NULL;
    guint32 a = 30;
    GdkAppLaunchContext* context = NULL;

    context = gdk_display_get_app_launch_context (gdk_display_get_default());

    gdk_app_launch_context_set_screen (context, gdk_screen_get_default());
    gdk_app_launch_context_set_timestamp (context, &a);

    if (!g_app_info_launch_default_for_uri ("http://www.gtk.org", context, &error))
        g_warning ("Launching failed: %s\n", error->message);

    g_object_unref (context);
}
