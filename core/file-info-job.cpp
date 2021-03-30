//
// Created by dingjing on 2021/3/30.
//

#include "file-info-job.h"

#include "file-info-p.h"

namespace dingjing
{
    FileInfoJob::FileInfoJob(FilePathList paths): Job(), paths_{std::move(paths)} {}

    void FileInfoJob::exec()
    {
        for(const auto& path: paths_) {
            if(isCancelled()) {
                break;
            }
            currentPath_ = path;

            bool retry;
            do {
                retry = false;
                GErrorPtr err;
                GFileInfoPtr inf{
                        g_file_query_info(path.gfile().get(), defaultGFileInfoQueryAttribs,
                                          G_FILE_QUERY_INFO_NONE, cancellable().get(), &err),
                        false
                };
                if(inf) {
                    auto fileInfoPtr = std::make_shared<FileInfo>(inf, path);
                    results_.push_back(fileInfoPtr);
                    Q_EMIT gotInfo(path, results_.back());
                }
                else {
                    auto act = emitError(err);
                    if(act == Job::ErrorAction::RETRY) {
                        retry = true;
                    }
                }
            } while(retry && !isCancelled());
        }
    }
}