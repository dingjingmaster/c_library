/*************************************************************************
> FileName: gtk4_1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年01月17日 星期五 18时02分40秒
 ************************************************************************/
#include <stdio.h>
#include <gtk/gtk.h>

gboolean supports_alpha = FALSE;
static gboolean main_expose_draw_cb (GtkWidget* widget, GdkEventExpose* event, gpointer udata);
static void main_screen_changed_cb (GtkWidget* widget, GdkScreen* old_screen, gpointer udata);

static gboolean main_expose_draw_cb (GtkWidget* widget, GdkEventExpose* event, gpointer udata)
{
    printf("draw\n");
    cairo_t* cr = gdk_cairo_create(gtk_widget_get_window(widget));
    if (supports_alpha) {
        //printf ("支持透明度设置\n");
        cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.0);
    } else {
        //printf("不支持透明度设置\n");
        cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    }

    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint(cr);

    cairo_destroy(cr);

    return FALSE;
}
static void main_screen_changed_cb (GtkWidget* widget, GdkScreen* old_screen, gpointer udata)
{
    GdkScreen* screen = gtk_widget_get_screen(widget);
    GdkVisual* visual = gdk_screen_get_rgba_visual(screen);
    if (!visual) {
        visual = gdk_screen_get_system_visual(screen);
        supports_alpha = FALSE;
    } else {
        supports_alpha = TRUE;
    }

    printf ("支持透明度否:%s\n", supports_alpha?"true":"false");
    gtk_widget_set_visible(widget, visual);
}

int main(int argc, char* argv[])
{
    gtk_init (&argc, &argv);

    GtkWidget* main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//    gtk_window_set_decorated(GTK_WINDOW(main_window), FALSE);
    gtk_widget_set_app_paintable(GTK_WIDGET(main_window), TRUE);
    gtk_widget_set_opacity(main_window, 0.0);

    g_signal_connect(G_OBJECT(main_window), "delete-event", gtk_main_quit, NULL);
    g_signal_connect(G_OBJECT(main_window), "draw", G_CALLBACK(main_expose_draw_cb), NULL);
    g_signal_connect(G_OBJECT(main_window), "screen-changed", G_CALLBACK(main_screen_changed_cb), NULL);

    main_screen_changed_cb(main_window, NULL, NULL);

    gtk_widget_show_all(main_window);

    gtk_main();

    return 0;
}
