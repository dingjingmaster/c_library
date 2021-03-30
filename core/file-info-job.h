//
// Created by dingjing on 2021/3/30.
//

#ifndef LIBCORE_DINGJING_FILEINFOJOB_H
#define LIBCORE_DINGJING_FILEINFOJOB_H

#include "job.h"
#include "global.h"
#include "file-path.h"
#include "file-info.h"
#include "smart-ptr.hpp"

namespace dingjing
{
    class FileInfoJob : public Job
    {
        Q_OBJECT
    public:

        explicit FileInfoJob(FilePathList paths);

        const FilePathList& paths() const
        {
            return paths_;
        }

        const FileInfoList& files() const
        {
            return results_;
        }

        const FilePath& currentPath() const
        {
            return currentPath_;
        }

    Q_SIGNALS:
        void gotInfo(const FilePath& path, std::shared_ptr<const FileInfo>& info);

    protected:
        void exec() override;

    private:
        FilePathList paths_;
        FileInfoList results_;
        FilePath currentPath_;
    };
}




#endif //LIBCORE_DINGJING_FILEINFOJOB_H
