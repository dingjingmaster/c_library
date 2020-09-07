/*************************************************************************
> FileName: clib-file-enumerator.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年09月07日 星期一 21时01分32秒
 ************************************************************************/
#ifndef _CLIBFILEENUMERATOR_H
#define _CLIBFILEENUMERATOR_H
#include <gio/gio.h>

/**
 * @brief 文件夹子文件枚举操作
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _file_enumerator_t file_enumerator_t;

file_enumerator_t* file_enumerator_new (char* uri);

void* file_enumerator_get_children (file_enumerator_t* enumerator);

void file_enumerator_free (file_enumerator_t* enumerator);

#ifdef __cplusplus
}
#endif
#endif
