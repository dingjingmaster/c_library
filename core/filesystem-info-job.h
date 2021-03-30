//
// Created by dingjing on 2021/3/30.
//

#ifndef LIBCORE_DINGJING_FILESYSTEMINFOJOB_H
#define LIBCORE_DINGJING_FILESYSTEMINFOJOB_H

#include "job.h"
#include "global.h"
#include "file-path.h"

namespace dingjing
{
    class FilesystemInfoJob : public Job
    {
        Q_OBJECT
    public:
        explicit FilesystemInfoJob(const FilePath& path):
                path_{path},
                isAvailable_{false},
                size_{0},
                freeSize_{0} {
        }

        bool isAvailable() const {
            return isAvailable_;
        }

        uint64_t size() const {
            return size_;
        }

        uint64_t freeSize() const {
            return freeSize_;
        }

    protected:

        void exec() override;

    private:
        FilePath path_;
        bool isAvailable_;
        uint64_t size_;
        uint64_t freeSize_;
    };
}

#endif //LIBCORE_DINGJING_FILESYSTEMINFOJOB_H
