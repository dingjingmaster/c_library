#ifndef PROXYFOLDERMODEL_H
#define PROXYFOLDERMODEL_H

#include <QList>
#include <QCollator>
#include <QSortFilterProxyModel>

#include "global.h"
#include "file-info.h"


namespace dingjing
{
class FolderModelItem;
class ProxyFolderModel;


class EXPORT_IMPORT_LIBRARY ProxyFolderModelFilter
{
public:
    virtual bool filterAcceptsRow(const ProxyFolderModel* model, const std::shared_ptr<const FileInfo>& info) const = 0;
    virtual ~ProxyFolderModelFilter() {}
};


class EXPORT_IMPORT_LIBRARY ProxyFolderModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ProxyFolderModel(QObject* parent = nullptr);
    ~ProxyFolderModel() override;

    void setSourceModel(QAbstractItemModel* model) override;

    bool showHidden() const;
    bool backupAsHidden() const;
    void setShowHidden(bool show);
    void setBackupAsHidden(bool backupAsHidden);

    bool hiddenLast();
    bool folderFirst();
    int thumbnailSize();
    bool showThumbnails();
    void setThumbnailSize(int size);
    void setShowThumbnails(bool show);
    void setHiddenLast(bool hiddenLast);
    void setFolderFirst(bool folderFirst);
    void setSortCaseSensitivity(Qt::CaseSensitivity cs);

    QModelIndex indexFromPath(const FilePath& path) const;
    std::shared_ptr<const FileInfo> fileInfoFromPath(const FilePath& path) const;
    std::shared_ptr<const FileInfo> fileInfoFromIndex(const QModelIndex& index) const;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void updateFilters();
    void addFilter(ProxyFolderModelFilter* filter);
    void removeFilter(ProxyFolderModelFilter* filter);

Q_SIGNALS:
    void sortFilterChanged();

protected Q_SLOTS:
    void onThumbnailLoaded(const QModelIndex& srcIndex, int size);

protected:
    bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;
    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

private:
    int                                 mThumbnailSize;
    bool                                mShowHidden;
    bool                                mBackupAsHidden;
    bool                                mFolderFirst;
    bool                                mHiddenLast;
    bool                                mShowThumbnails;
    QCollator                           mCollator;
    QList<ProxyFolderModelFilter*>      mFilters;
};

}


#endif // PROXYFOLDERMODEL_H
