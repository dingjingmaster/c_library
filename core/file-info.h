//
// Created by dingjing on 2021/3/30.
//

#ifndef LIBCORE_DINGJING_FILEINFO_H
#define LIBCORE_DINGJING_FILEINFO_H

#include <set>
#include <vector>
#include <string>
#include <utility>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <forward_list>

#include <QObject>
#include <QtGlobal>

#include "global.h"
#include "file-path.h"
#include "icon-info.h"
#include "mime-type.h"
#include "smart-ptr.hpp"

namespace dingjing
{
    class FileInfoList;
    typedef std::set<unsigned int> HashSet;

    class EXPORT_IMPORT_LIBRARY FileInfo
    {
    public:

        explicit FileInfo();

        explicit FileInfo(const GFileInfoPtr& inf, const FilePath& filePath, const FilePath& parentDirPath = FilePath());

        virtual ~FileInfo();

        bool canSetHidden() const
        {
            return isHiddenChangeable_;
        }

        bool canSetIcon() const
        {
            return isIconChangeable_;
        }

        bool canSetName() const
        {
            return isNameChangeable_;
        }

        bool canThumbnail() const;

        gid_t gid() const
        {
            return gid_;
        }

        uid_t uid() const
        {
            return uid_;
        }

        const char* filesystemId() const
        {
            return filesystemId_;
        }

        const std::shared_ptr<const IconInfo>& icon() const
        {
            return icon_;
        }

        const std::shared_ptr<const MimeType>& mimeType() const
        {
            return mimeType_;
        }

        quint64 ctime() const
        {
            return ctime_;
        }

        quint64 crtime() const
        {
            return crtime_;
        }

        quint64 atime() const
        {
            return atime_;
        }

        quint64 mtime() const
        {
            return mtime_;
        }

        quint64 dtime() const
        {
            return dtime_;
        }
        const std::string& target() const
        {
            return target_;
        }

        bool isWritableDirectory() const
        {
            return (!isReadOnly_ && isDir());
        }

        bool isAccessible() const
        {
            return isAccessible_;
        }

        bool isWritable() const
        {
            return isWritable_;
        }

        bool isDeletable() const
        {
            return isDeletable_;
        }

        bool isExecutableType() const;

        bool isBackup() const
        {
            return isBackup_;
        }

        bool isHidden() const
        {
            // FIXME: we might treat backup files as hidden
            return isHidden_;
        }

        bool isUnknownType() const
        {
            return mimeType_->isUnknownType();
        }

        bool isDesktopEntry() const
        {
            return mimeType_->isDesktopEntry();
        }

        bool isText() const
        {
            return mimeType_->isText();
        }

        bool isImage() const
        {
            return mimeType_->isImage();
        }

        bool isMountable() const
        {
            return isMountable_;
        }

        bool isShortcut() const
        {
            return isShortcut_;
        }

        bool isSymlink() const
        {
            return S_ISLNK(mode_) ? true : false;
        }

        bool isDir() const
        {
            return S_ISDIR(mode_) || mimeType_->isDir();
        }

        bool isNative() const
        {
            return dirPath_ ? dirPath_.isNative() : path().isNative();
        }

        mode_t mode() const
        {
            return mode_;
        }

        uint64_t realSize() const
        {
            return blksize_ *blocks_;
        }

        uint64_t size() const
        {
            return size_;
        }

        const std::string& name() const
        {
            return name_;
        }

        const QString& displayName() const
        {
            return dispName_;
        }

        QString description() const
        {
            return QString::fromUtf8(mimeType_ ? mimeType_->desc() : "");
        }

        FilePath path() const
        {
            return filePath_ ? filePath_ : dirPath_ ? dirPath_.child(name_.c_str()) : FilePath::fromPathStr(name_.c_str());
        }

        const FilePath& dirPath() const
        {
            return dirPath_;
        }

        void setFromGFileInfo(const GFileInfoPtr& inf, const FilePath& filePath, const FilePath& parentDirPath);

        const std::forward_list<std::shared_ptr<const IconInfo>>& emblems() const
        {
            return emblems_;
        }

        bool isTrustable() const;

        void setTrustable(bool trust) const;

        GObjectPtr<GFileInfo> gFileInfo() const
        {
            return inf_;
        }

    private:
        GObjectPtr<GFileInfo> inf_;
        std::string name_;
        QString dispName_;

        FilePath filePath_;
        FilePath dirPath_;

        mode_t mode_;
        const char* filesystemId_;
        uid_t uid_;
        gid_t gid_;
        uint64_t size_;
        quint64 mtime_;
        quint64 atime_;
        quint64 ctime_;
        quint64 crtime_;
        quint64 dtime_;

        uint64_t blksize_;
        uint64_t blocks_;

        std::shared_ptr<const MimeType> mimeType_;
        std::shared_ptr<const IconInfo> icon_;
        std::forward_list<std::shared_ptr<const IconInfo>> emblems_;

        std::string target_; /* target of shortcut or mountable. */

        bool isShortcut_ : 1; /* TRUE if file is shortcut type */
        bool isMountable_ : 1; /* TRUE if file is mountable type */
        bool isAccessible_ : 1; /* TRUE if can be read by user */
        bool isWritable_ : 1; /* TRUE if can be written to by user */
        bool isDeletable_ : 1; /* TRUE if can be deleted by user */
        bool isHidden_ : 1; /* TRUE if file is hidden */
        bool isBackup_ : 1; /* TRUE if file is backup */
        bool isNameChangeable_ : 1; /* TRUE if name can be changed */
        bool isIconChangeable_ : 1; /* TRUE if icon can be changed */
        bool isHiddenChangeable_ : 1; /* TRUE if hidden can be changed */
        bool isReadOnly_ : 1; /* TRUE if host FS is R/O */
    };

    class EXPORT_IMPORT_LIBRARY FileInfoList: public std::vector<std::shared_ptr<const FileInfo>>
    {
    public:

        bool isSameType() const;

        bool isSameFilesystem() const;

        FilePathList paths() const
        {
            FilePathList ret;
            for(auto& file: *this) {
                ret.push_back(file->path());
            }
            return ret;
        }
    };

    typedef std::shared_ptr<const FileInfo> FileInfoPtr;

    typedef std::pair<FileInfoPtr, FileInfoPtr> FileInfoPair;

}

Q_DECLARE_METATYPE(std::shared_ptr<const dingjing::FileInfo>)

#endif //LIBCORE_DINGJING_FILEINFO_H
