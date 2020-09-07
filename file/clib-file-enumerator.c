/*************************************************************************
> FileName: clib-file-enumerator.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年09月07日 星期一 21时08分43秒
 ************************************************************************/
#include "clib-file-enumerator.h"

#include <stdio.h>
#include <string.h>

struct _file_enumerator_t
{
    char*               uri;
    GFile*              file;
    GError*             error;
    GList*              children;
    GCancellable*       cancellable;
    GFileEnumerator*    enumerator;
};

file_enumerator_t* file_enumerator_new (char* uri)
{
    file_enumerator_t* enumerator = NULL;

    enumerator = (file_enumerator_t*) malloc (sizeof(file_enumerator_t));
    if (NULL == enumerator) {
        return NULL;
    }

    memset (enumerator, 0, sizeof (file_enumerator_t));

    enumerator->cancellable = g_cancellable_new ();

    if (g_str_has_prefix (uri, "file://")) {
        enumerator->uri = g_strdup(uri);
    } else if (g_str_has_prefix (uri, "/")) {
        enumerator->uri = g_strdup_printf ("file://%s", uri);
    }

    if (NULL != enumerator->uri) {
        enumerator->file = g_file_new_for_uri (enumerator->uri);
    }

    return enumerator;
}

void* file_enumerator_get_children (file_enumerator_t* enumerator)
{
    GFileInfo* info = NULL;

    if (NULL == enumerator) {
        return NULL;
    }

    if (NULL != enumerator->children) {
        g_list_free_full (enumerator->children, g_object_unref);
    }

    enumerator->children = NULL;

    if (NULL != enumerator->file) {
        enumerator->enumerator = g_file_enumerate_children (enumerator->file,
                "standard::*", G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS,
                enumerator->cancellable, &(enumerator->error));
        for (g_file_enumerator_iterate (enumerator->enumerator, &info, NULL, NULL, NULL); NULL != info;
                g_file_enumerator_iterate (enumerator->enumerator, &info, NULL, NULL, NULL)) {
            enumerator->children = g_list_append (enumerator->children, g_object_ref(info));
        }
        g_file_enumerator_close_async (enumerator->enumerator, 0, NULL, NULL, NULL);
    }

    return enumerator->children;
}

void file_enumerator_free (file_enumerator_t* enumerator)
{
    if (NULL != enumerator->uri) {
        g_free (enumerator->uri);
    }

    if (NULL != enumerator->error) {
        g_error_free (enumerator->error);
    }

    if (NULL != enumerator->cancellable) {
        g_object_unref (enumerator->cancellable);
    }

    if (NULL != enumerator->file) {
        g_object_unref (enumerator->file);
    }

    if (NULL != enumerator->children) {
        g_list_free_full (enumerator->children, g_object_unref);
    }

    if (NULL != enumerator->enumerator) {
        g_object_unref (enumerator->enumerator);
    }
}

