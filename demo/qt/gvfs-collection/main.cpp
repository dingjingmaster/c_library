#include "favorites-vfs-file.h"

#include <QtCore>
#include <stdio.h>
#include <QString>

static GFile* test_vfs_parse_name (GVfs* vfs, const char* parse_name, gpointer user_data)
{
    QString tmp = parse_name;
    if (tmp.contains("real-uri:")) {
        QString realUri = tmp.split("real-uri:").last();
        return g_file_new_for_uri(realUri.toUtf8().constData());
    }

    return vfs_favorites_file_new_for_uri(parse_name);
}

static GFile* test_vfs_lookup (GVfs* vfs, const char* uri, gpointer user_data)
{
    return test_vfs_parse_name(vfs, uri, user_data);
}

int main(int argc, char *argv[])
{
    GVfs* vfs = g_vfs_get_default();

    const gchar* const* schemas = g_vfs_get_supported_uri_schemes(vfs);
    const gchar* const* ps = schemas;

    while (*ps) {
        printf("schemas: %s\n", *ps);
        ++ps;
    }

    printf("=======================================\n\n");

#if GLIB_CHECK_VERSION(2, 50, 0)
    gboolean res = g_vfs_register_uri_scheme(vfs, "favorite", test_vfs_lookup, NULL, NULL, test_vfs_parse_name, NULL, NULL);
#endif

    schemas = g_vfs_get_supported_uri_schemes(vfs);
    ps = schemas;

    while (*ps) {
        printf("schemas: %s\n", *ps);
        ++ps;
    }

}
