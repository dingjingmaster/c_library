/*************************************************************************
> FileName: command_line.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月23日 星期一 21时12分36秒
 ************************************************************************/
#include <glib.h>

static gint repeats = 2;
static gint max_size = 8;
static gboolean verbose = FALSE;
static gboolean beep = FALSE;
static gboolean randomize = FALSE;
static char* c = NULL;

static GOptionEntry entries[] = {
    {"repeats", 'r', 0, G_OPTION_ARG_INT, &repeats, "Average over N repetitions", "N"},
    {"max-size", 'm', 0, G_OPTION_ARG_INT, &max_size, "Test up to 2^M items", "M"},
    {"verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Be verbose", NULL},
    {"beep", 'b', 0, G_OPTION_ARG_NONE, &beep, "Beep whe done", NULL},
    {"string", 's', 0, G_OPTION_ARG_STRING, &c, "string", NULL},
    {"rand", 0, 0, G_OPTION_ARG_NONE, &randomize, "Randomize the data", NULL},
    {NULL}
};

int main (int argc, char* argv[])
{
    GError* error = NULL;
    GOptionContext* context = NULL;

    context = g_option_context_new ("............");

    g_option_context_add_main_entries (context, entries, NULL);

    if (!g_option_context_parse(context, &argc, &argv, &error)) {
        g_print ("option parsing failed:%s\n", error->message);
        exit(1);
    }

    printf ("repeats:%d\n", repeats);
    printf ("max-size:%d\n", max_size);
    printf ("string:%s\n", c);
}

