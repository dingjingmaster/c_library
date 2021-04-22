#ifndef FOLDERMODEL_H
#define FOLDERMODEL_H

#include "global.h"
#include "folder.h"
#include "thumbnailer.h"
#include "folder-model-item.h"
#include "thumbnail-job.h"

#include <QIcon>
#include <QList>
#include <QImage>
#include <vector>
#include <utility>
#include <forward_list>
#include <QItemSelection>
#include <QAbstractListModel>

namespace dingjing
{

class EXPORT_IMPORT_LIBRARY FolderModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Role
    {
        FileInfoRole = Qt::UserRole,
        FileIsDirRole,
        FileIsCutRole
    };
    enum ColumnId
    {
        ColumnFileName,
        ColumnFileType,
        ColumnFileSize,
        ColumnFileMTime,
        ColumnFileCrTime,
        ColumnFileDTime,
        ColumnFileOwner,
        ColumnFileGroup,
        NumOfColumns
    };

public:
    explicit FolderModel();
    ~FolderModel() override;
    const std::shared_ptr<Folder>& folder() const
    {
        return mFolder;
    }

    void setFolder(const std::shared_ptr<Folder>& newFolder);

    FilePath path()
    {
        return mFolder ? mFolder->path() : FilePath();
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    // void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    QStringList mimeTypes() const override;
    QMimeData* mimeData(const QModelIndexList& indexes) const override;
    Qt::DropActions supportedDropActions() const override;
    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;

    std::shared_ptr<const FileInfo> fileInfoFromIndex(const QModelIndex& index) const;
    std::shared_ptr<const FileInfo> fileInfoFromPath(const FilePath& path) const;
    FolderModelItem* itemFromIndex(const QModelIndex& index) const;
    QImage thumbnailFromIndex(const QModelIndex& index, int size);

    void cacheThumbnails(int size);
    void releaseThumbnails(int size);

    void setShowFullName(bool fullName)
    {
        mShowFullNames = fullName;
    }

Q_SIGNALS:
    void thumbnailLoaded(const QModelIndex& index, int size);
    void fileSizeChanged(const QModelIndex& index);
    void filesAdded(FileInfoList infoList);

protected Q_SLOTS:
    void onStartLoading();
    void onFinishLoading();
    void onFilesAdded(const FileInfoList& files);
    void onFilesRemoved(const FileInfoList& files);
    void onFilesChanged(std::vector<FileInfoPair>& files);

    void loadPendingThumbnails();
    void onClipboardDataChange();
    void onThumbnailJobFinished();
    void onThumbnailLoaded(const std::shared_ptr<const FileInfo>& file, int size, const QImage& image);

protected:
    void removeAll();
    void insertFiles(int row, const FileInfoList& files);
    void queueLoadThumbnail(const std::shared_ptr<const FileInfo>& file, int size);

    QList<FolderModelItem>::iterator findItemByName(const char* name, int* row);
    QList<FolderModelItem>::iterator findItemByFileInfo(const FileInfo* info, int* row);

private:
    void updateCutFilesSet();
    QString makeTooltip(FolderModelItem* item) const;

private:
    struct ThumbnailData
    {
        ThumbnailData(int size) : mSize{size}, mRefCount{1} {}

        int             mSize;
        int             mRefCount;
        FileInfoList    mPendingThumbnails;
    };

    bool                                mIsLoaded = false;
    bool                                mHasCutfile = false;
    bool                                mShowFullNames = false;
    bool                                mHasPendingThumbnailHandler = false;

    HashSet                             mCutFilesHashSet;
    QList<FolderModelItem>              mItems;
    std::shared_ptr<Folder>             mFolder;

    std::vector<ThumbnailJob*>          mPendingThumbnailJobs;
    std::forward_list<ThumbnailData>    mThumbnailData;
};

}

#endif // FOLDERMODEL_H
