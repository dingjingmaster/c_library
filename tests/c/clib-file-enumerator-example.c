/*************************************************************************
> FileName: clib-file-enumerator-example.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年09月07日 星期一 22时37分55秒
 ************************************************************************/
#include "../../core/c/file/clib-file-enumerator.h"

#include <stdio.h>

int main (int argc, char* argv[])
{
    file_enumerator_t* enumerator = NULL;

    enumerator = file_enumerator_new ("/usr");
    
    GList* c = (GList*)file_enumerator_get_children (enumerator);
    for (GList* i = c; NULL != i; i = i->next) {
        GFileInfo* info = (GFileInfo*)(i->data);
        if (NULL == info) continue;
        printf ("%s\n", g_file_info_get_attribute_as_string (info, G_FILE_ATTRIBUTE_STANDARD_NAME));
    }

    file_enumerator_free (enumerator);
    return 0;
}
