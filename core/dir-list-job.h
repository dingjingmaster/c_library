//
// Created by dingjing on 2021/3/30.
//

#ifndef LIBCORE_DINGJING_DIRLISTJOB_H
#define LIBCORE_DINGJING_DIRLISTJOB_H

#include "job.h"
#include "global.h"
#include "file-path.h"
#include "file-info.h"
#include "smart-ptr.hpp"

#include <mutex>

namespace dingjing
{
    class DirListJob : public Job
    {
        Q_OBJECT
    public:
        enum Flags {
            FAST = 0,
            DIR_ONLY = 1 << 0,
            DETAILED = 1 << 1
        };

        explicit DirListJob(const FilePath& path, Flags flags);

        FileInfoList& files() {
            return files_;
        }

        void setIncremental(bool set);

        bool incremental() const {
            return emit_files_found;
        }

        FilePath dirPath() const {
            std::lock_guard<std::mutex> lock{mutex_};
            return dir_path;
        }

        std::shared_ptr<const FileInfo> dirInfo() const {
            std::lock_guard<std::mutex> lock{mutex_};
            return dir_fi;
        }

    Q_SIGNALS:
        void filesFound(FileInfoList& foundFiles);

    protected:

        void exec() override;

    private:
        mutable std::mutex mutex_;
        FilePath dir_path;
        Flags flags;
        std::shared_ptr<const FileInfo> dir_fi;
        FileInfoList files_;
        bool emit_files_found;
        // guint delay_add_files_handler;
        // GSList* files_to_add;
    };
}



#endif //LIBCORE_DINGJING_DIRLISTJOB_H
