//
// Created by dingjing on 2021/3/30.
//

#ifndef LIBCORE_DINGJING_FOLDER_H
#define LIBCORE_DINGJING_FOLDER_H

#include <mutex>
#include <memory>
#include <vector>
#include <cstdint>
#include <gio/gio.h>
#include <functional>
#include <unordered_map>

#include <QObject>
#include <QtGlobal>

#include "job.h"
#include "global.h"
#include "file-info.h"
#include "smart-ptr.hpp"
#include "volume-manager.h"
#include "filesystem-info-job.h"

namespace dingjing
{
    class DirListJob;
    class FileInfoJob;
    class FileSystemInfoJob;

    class EXPORT_IMPORT_LIBRARY Folder : public QObject
    {
        Q_OBJECT
    public:

        explicit Folder();

        explicit Folder(const FilePath& path);

        ~Folder() override;

        static std::shared_ptr<Folder> fromPath(const FilePath& path);

        static std::shared_ptr<Folder> findByPath(const FilePath& path);

        bool makeDirectory(const char* name, GError** error);

        void queryFilesystemInfo();

        bool getFilesystemInfo(uint64_t* total_size, uint64_t* free_size) const;

        void reload();

        bool isIncremental() const;

        bool isValid() const;

        bool isLoaded() const;

        std::shared_ptr<const FileInfo> fileByName(const char* name) const;

        bool isEmpty() const;

        bool hasFileMonitor() const;

        FileInfoList files() const;

        const FilePath& path() const;

        const std::shared_ptr<const FileInfo> &info() const;

        void forEachFile(std::function<void (const std::shared_ptr<const FileInfo>&)> func) const
        {
            std::lock_guard<std::mutex> lock{mutex_};
            for(auto it = files_.begin(); it != files_.end(); ++it) {
                func(it->second);
            }
        }

    Q_SIGNALS:
        void startLoading();

        void finishLoading();

        void filesAdded(FileInfoList& addedFiles);

        void filesChanged(std::vector<FileInfoPair>& changePairs);

        void filesRemoved(FileInfoList& removedFiles);

        void removed();

        void changed();

        void unmount();

        void contentChanged();

        void fileSystemChanged();

        // FIXME: this API design is bad. We leave this here to be compatible with the old libfm C API.
        // It might be better to remember the error state while loading the folder, and let the user of the
        // API handle the error on finish.
        void error(const GErrorPtr& err, Job::ErrorSeverity severity, Job::ErrorAction& response);

    private:

        static void _onFileChangeEvents(GFileMonitor* monitor, GFile* file, GFile* other_file, GFileMonitorEvent event_type, Folder* _this)
        {
            _this->onFileChangeEvents(monitor, file, other_file, event_type);
        }
        void onFileChangeEvents(GFileMonitor* monitor, GFile* file, GFile* other_file, GFileMonitorEvent event_type);
        void onDirChanged(GFileMonitorEvent event_type);

        void queueUpdate();
        void queueReload();

        bool eventFileAdded(const FilePath &path);
        bool eventFileChanged(const FilePath &path);
        void eventFileDeleted(const FilePath &path);

    private Q_SLOTS:

        void processPendingChanges();

        void onDirListFinished();

        void onFileSystemInfoFinished();

        void onFileInfoFinished();

        void onIdleReload();

        void onMountAdded(const Mount& mnt);

        void onMountRemoved(const Mount& mnt);

    private:
        FilePath dirPath_;
        GFileMonitorPtr dirMonitor_;

        std::shared_ptr<const FileInfo> dirInfo_;
        DirListJob* dirlist_job;
        std::vector<FileInfoJob*> fileinfoJobs_;
        FilesystemInfoJob* fsInfoJob_;

        std::shared_ptr<VolumeManager> volumeManager_;

        /* for file monitor */
        bool has_idle_reload_handler;
        bool has_idle_update_handler;
        FilePathList paths_to_add;
        FilePathList paths_to_update;
        FilePathList paths_to_del;
        // GSList* pending_jobs;
        bool pending_change_notify;
        bool filesystem_info_pending;

        bool wants_incremental;
        bool stop_emission; /* don't set it 1 bit to not lock other bits */

        // NOTE: Here, FileInfo::path().baseName().get() should be used as the key value, not FileInfo::name(),
        // because the latter is not always the same as the former and the former will be used for comparison.
        std::unordered_map<const std::string, std::shared_ptr<const FileInfo>, std::hash<std::string>> files_;

        /* filesystem info - set in query thread, read in main */
        uint64_t fs_total_size;
        uint64_t fs_free_size;
        GCancellablePtr fs_size_cancellable;

        bool has_fs_info : 1;
        bool defer_content_test : 1;

        static std::unordered_map<FilePath, std::weak_ptr<Folder>, FilePathHash> cache_;
        static std::mutex mutex_;
    };
}



#endif //LIBCORE_DINGJING_FOLDER_H
