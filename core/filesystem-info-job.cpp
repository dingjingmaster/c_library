//
// Created by dingjing on 2021/3/30.
//

#include "filesystem-info-job.h"
#include "smart-ptr.hpp"

namespace dingjing
{
    void FilesystemInfoJob::exec()
    {
        GObjectPtr<GFileInfo> inf = GObjectPtr<GFileInfo>{
                g_file_query_filesystem_info(
                        path_.gfile().get(),
                        G_FILE_ATTRIBUTE_FILESYSTEM_SIZE","
                        G_FILE_ATTRIBUTE_FILESYSTEM_FREE,
                        cancellable().get(), nullptr),
                false
        };
        if(!inf)
            return;
        if(g_file_info_has_attribute(inf.get(), G_FILE_ATTRIBUTE_FILESYSTEM_SIZE)) {
            size_ = g_file_info_get_attribute_uint64(inf.get(), G_FILE_ATTRIBUTE_FILESYSTEM_SIZE);
            freeSize_ = g_file_info_get_attribute_uint64(inf.get(), G_FILE_ATTRIBUTE_FILESYSTEM_FREE);
            isAvailable_ = true;
        }
    }
}