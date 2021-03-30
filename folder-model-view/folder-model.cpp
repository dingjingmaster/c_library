#include "folder-model.h"

#include <QUrl>
#include <QTimer>
#include <QString>
#include <QVector>
#include <QPixmap>
#include <QPainter>
#include <iostream>
#include <QDateTime>
#include <algorithm>
#include <QMimeData>
#include <QClipboard>
#include <QByteArray>
#include <qmimedata.h>
#include <QtAlgorithms>
#include <QApplication>
#include <core/utils.h>

//#include "utilities.h"
//#include "fileoperation.h"

namespace dingjing
{

FolderModel::FolderModel() : mHasPendingThumbnailHandler{false}, mShowFullNames{false}, mIsLoaded{false}, mHasCutfile{false}
{
    connect(QApplication::clipboard(), &QClipboard::dataChanged, this, &FolderModel::onClipboardDataChange);
}

FolderModel::~FolderModel()
{
    // if the thumbnail requests list is not empty, cancel them
    for(auto job: mPendingThumbnailJobs) {
        job->cancel();
    }
}

void FolderModel::setFolder(const std::shared_ptr<Folder>& newFolder)
{
    if(mFolder) {
        removeAll();        // remove old items
    }

    if(newFolder) {
        mFolder = newFolder;
        connect(mFolder.get(), &Folder::startLoading, this, &FolderModel::onStartLoading);
        connect(mFolder.get(), &Folder::finishLoading, this, &FolderModel::onFinishLoading);
        connect(mFolder.get(), &Folder::filesAdded, this, &FolderModel::onFilesAdded);
        connect(mFolder.get(), &Folder::filesChanged, this, &FolderModel::onFilesChanged);
        connect(mFolder.get(), &Folder::filesRemoved, this, &FolderModel::onFilesRemoved);
        // handle the case if the folder is already loaded
        if(mFolder->isLoaded()) {
            mIsLoaded = true;
            insertFiles(0, mFolder->files());
            onClipboardDataChange(); // files may have been cut
        }
    }
}

void FolderModel::onStartLoading()
{
    mIsLoaded = false;
    mHasCutfile = false;
    removeAll();
}

void FolderModel::onFinishLoading()
{
    mIsLoaded = true;
    onClipboardDataChange();
}

void FolderModel::onFilesAdded(const FileInfoList& files)
{
    int nFiles = files.size();
    beginInsertRows(QModelIndex(), mItems.count(), mItems.count() + nFiles - 1);
    for(auto& info : files) {
        FolderModelItem item(info);

        // cut files may be removed and added again
        if(mIsLoaded && mCutFilesHashSet.count(info->path().hash()) != 0) {
            item.mIsCut = true;
            mHasCutfile = true;
        }

        mItems.append(item);
    }
    endInsertRows();

    if(mIsLoaded) {
        Q_EMIT filesAdded(files);
    }
}

void FolderModel::onFilesChanged(std::vector<FileInfoPair>& files)
{
    for(auto& change : files) {
        int row;
        auto& oldInfo = change.first;
        auto& newInfo = change.second;
        QList<FolderModelItem>::iterator it = findItemByFileInfo(oldInfo.get(), &row);
        if(it != mItems.end()) {
            FolderModelItem& item = *it;
            // try to update the item
            item.mInfo = newInfo;
            item.mThumbnails.clear();
            QModelIndex index = createIndex(row, 0, &item);
            Q_EMIT dataChanged(index, index);
            if(oldInfo->size() != newInfo->size()) {
                Q_EMIT fileSizeChanged(index);
            }
        }
    }
}

void FolderModel::onFilesRemoved(const FileInfoList& files)
{
    for(auto& info : files) {
        int row;
        QList<FolderModelItem>::iterator it = findItemByName(info->name().c_str(), &row);
        if(it != mItems.end()) {
            beginRemoveRows(QModelIndex(), row, row);
            mItems.erase(it);
            endRemoveRows();
        }
    }
}

void FolderModel::loadPendingThumbnails()
{
    mHasPendingThumbnailHandler = false;
    for(auto& item: mThumbnailData) {
        if(!item.mPendingThumbnails.empty()) {
            auto job = new ThumbnailJob(std::move(item.mPendingThumbnails), item.mSize);
            mPendingThumbnailJobs.push_back(job);
            job->setAutoDelete(true);
            connect(job, &ThumbnailJob::thumbnailLoaded, this, &FolderModel::onThumbnailLoaded, Qt::BlockingQueuedConnection);
            connect(job, &ThumbnailJob::finished, this, &FolderModel::onThumbnailJobFinished, Qt::BlockingQueuedConnection);
            ThumbnailJob::threadPool()->start(job);
        }
    }
}

void FolderModel::queueLoadThumbnail(const std::shared_ptr<const FileInfo>& file, int size)
{
    auto it = std::find_if(mThumbnailData.begin(), mThumbnailData.end(), [size](ThumbnailData& item){return item.mSize == size;});
    if(it != mThumbnailData.end()) {
        it->mPendingThumbnails.push_back(file);
        if(!mHasPendingThumbnailHandler) {
            QTimer::singleShot(0, this, &FolderModel::loadPendingThumbnails);
            mHasPendingThumbnailHandler = true;
        }
    }
}

void FolderModel::insertFiles(int row, const FileInfoList& files)
{
    int n_files = files.size();
    beginInsertRows(QModelIndex(), row, row + n_files - 1);
    for(auto& info : files) {
        FolderModelItem item(info);
        mItems.append(item);
    }
    endInsertRows();
}

void FolderModel::updateCutFilesSet() {
    // NOTE: To process large numbers of files as fast as possible,
    // we analyze the clipboard data directly, without using "utilities.h",
    // because otherwise, we should iterate through path lists multiple times.

    if(mFolder == nullptr) {
        return;
    }

    mCutFilesHashSet.clear();

    bool cutPathFound = false;
    const QClipboard* clipboard = QApplication::clipboard();
    const QMimeData* data = clipboard->mimeData();

    // Gnome, LXDE, XFCE (see utilities.cpp -> pasteFilesFromClipboard)
    if(data->hasFormat(QStringLiteral("x-special/gnome-copied-files"))) {
        QByteArray gnomeData = data->data(QStringLiteral("x-special/gnome-copied-files"));
        char* pdata = gnomeData.data();
        char* eol = strchr(pdata, '\n');
        if(eol) {
            *eol = '\0';
            if(strcmp(pdata, "cut") == 0) {
                char** uris = g_strsplit_set(eol + 1, "\r\n", -1);
                for(char** uri = uris; *uri; ++uri) {
                    if(**uri != '\0') {
                        cutPathFound = true; // although it may not be in this folder
                        auto path = FilePath::fromUri(*uri);
                        if(path.parent() == mFolder->path()) {
                            mCutFilesHashSet.insert(path.hash());
                        }
                    }
                }
                g_strfreev(uris);
            }
        }
    }
    // KDE
    if(!cutPathFound && data->hasUrls()) {
        QByteArray cut = data->data(QStringLiteral("application/x-kde-cutselection"));
        if(!cut.isEmpty() && QChar::fromLatin1(cut.at(0)) == QLatin1Char('1')) {
            QList<QUrl> urls = data->urls();
            for(auto it = urls.cbegin(); it != urls.cend(); ++it) {
                auto path = FilePath::fromUri(it->toString().toUtf8().constData());
                if(path.parent() == mFolder->path()) {
                    mCutFilesHashSet.insert(path.hash());
                }
            }
        }
    }
}

void FolderModel::onClipboardDataChange()
{
    if(mFolder && mIsLoaded) {
        updateCutFilesSet();
        if(!mCutFilesHashSet.empty()) {
             // (some) files are cut here; so the items need to be updated
            mHasCutfile = false;
            QList<FolderModelItem>::iterator it = mItems.begin();
            while(it != mItems.end()) {
                FolderModelItem& item = *it;
                if(mCutFilesHashSet.count(it->mInfo->path().hash()) != 0) {
                    item.mIsCut = true;
                    mHasCutfile = true;
                }
                else {
                    item.mIsCut = false;
                }
                ++it;
            }
            Q_EMIT dataChanged(index(0, 0), index(rowCount() - 1, 0));
        } else if(mHasCutfile) {
            // this folder contained a cut file before but not anymore;
            // so its items need to be updated
            mHasCutfile = false;
            QList<FolderModelItem>::iterator it = mItems.begin();
            while(it != mItems.end()) {
                FolderModelItem& item = *it;
                item.mIsCut = false;
                ++it;
            }
            Q_EMIT dataChanged(index(0, 0), index(rowCount() - 1, 0));
        }
    }
}

void FolderModel::removeAll()
{
    if(mItems.empty()) {
        return;
    }
    beginRemoveRows(QModelIndex(), 0, mItems.size() - 1);
    mItems.clear();
    endRemoveRows();
}

int FolderModel::rowCount(const QModelIndex& parent) const
{
    if(parent.isValid()) {
        return 0;
    }
    return mItems.size();
}

int FolderModel::columnCount(const QModelIndex& parent = QModelIndex()) const
{
    if(parent.isValid()) {
        return 0;
    }
    return NumOfColumns;
}

FolderModelItem* FolderModel::itemFromIndex(const QModelIndex& index) const
{
    return reinterpret_cast<FolderModelItem*>(index.internalPointer());
}

std::shared_ptr<const FileInfo> FolderModel::fileInfoFromIndex(const QModelIndex& index) const
{
    FolderModelItem* item = itemFromIndex(index);
    return item ? item->mInfo : nullptr;
}

QString FolderModel::makeTooltip(FolderModelItem* item) const
{
    // name (bold)
    QString tip = QStringLiteral("<p><b>") + (mShowFullNames ? QString::fromStdString(item->name()) : item->displayName()).toHtmlEscaped() + QStringLiteral("</b></p>");

    // parent dir
    auto info = item->mInfo;
    auto parent_path = info->path().parent();
    auto parent_str = parent_path ? parent_path.displayName() : nullptr;
    if(parent_str) {
        tip += QStringLiteral("<p><i>") + tr("Location:") + QStringLiteral("</i> ") + QString::fromUtf8(parent_str.get()).toHtmlEscaped() + QStringLiteral("</p>");
    }

    // file type
    tip += QStringLiteral("<i>") + tr("File type:") + QStringLiteral("</i> ") + QString::fromUtf8(info->mimeType()->desc());

    // file size
    const QString dSize = item->displaySize();
    if(!dSize.isEmpty()) { // it's empty for directories
        tip += QStringLiteral("<br><i>") + tr("File size:") + QStringLiteral("</i> ") + dSize;
    }

    // times
    tip += QStringLiteral("<br><i>") + tr("Last modified:") + QStringLiteral("</i> ") + item->displayMtime()
           + QStringLiteral("<br><i>") + tr("Last accessed:") + QStringLiteral("</i> ")
                                       + (info->atime() == 0 ? tr("N/A") : QDateTime::fromMSecsSinceEpoch(info->atime() * 1000).toString(Qt::SystemLocaleShortDate))
           + QStringLiteral("<br><i>") + tr("Created:") + QStringLiteral("</i> ") + item->displayCrtime();

    // owner
    const QString owner = item->ownerName();
    if(!owner.isEmpty()) { // can be empty
        tip += QStringLiteral("<br><i>") + tr("Owner:") + QStringLiteral("</i> ") + owner
               + QStringLiteral("<br><i>") + tr("Group:") + QStringLiteral("</i> ") + item->ownerGroup();
    }

    return tip;
}

QVariant FolderModel::data(const QModelIndex& index, int role/* = Qt::DisplayRole*/) const
{
    if(!index.isValid() || index.row() > mItems.size() || index.column() >= NumOfColumns) {
        return QVariant();
    }
    FolderModelItem* item = itemFromIndex(index);
    auto info = item->mInfo;
    bool isCut = mFolder && item->mIsCut;

    switch(role) {
    case Qt::ToolTipRole:
        return QVariant(makeTooltip(item));
    case Qt::DisplayRole:  {
        switch(index.column()) {
        case ColumnFileName:
            return (mShowFullNames && !item->name().empty() ? QString::fromStdString(item->name()) : item->displayName());
        case ColumnFileType:
            return QString::fromUtf8(info->mimeType()->desc());
        case ColumnFileMTime:
            return item->displayMtime();
        case ColumnFileCrTime:
            return item->displayCrtime();
        case ColumnFileDTime:
            return item->displayDtime();
        case ColumnFileSize:
            return item->displaySize();
        case ColumnFileOwner:
            return item->ownerName();
        case ColumnFileGroup:
            return item->ownerGroup();
        }
        break;
    }
    case Qt::DecorationRole: {
        if(index.column() == 0) {
            return QVariant(item->icon());
        }
        break;
    }
    case Qt::EditRole: {
        if(index.column() == 0) {
            return QString::fromStdString(info->name());
        }
        break;
    }
    case FileInfoRole:
        return QVariant::fromValue(info);
    case FileIsDirRole:
        return QVariant(info->isDir());
    case FileIsCutRole:
        return isCut;
    }
    return QVariant();
}

QVariant FolderModel::headerData(int section, Qt::Orientation orientation, int role/* = Qt::DisplayRole*/) const
{
    if(role == Qt::DisplayRole) {
        if(orientation == Qt::Horizontal) {
            QString title;
            switch(section) {
            case ColumnFileName:
                title = tr("Name");
                break;
            case ColumnFileType:
                title = tr("Type");
                break;
            case ColumnFileSize:
                title = tr("Size");
                break;
            case ColumnFileMTime:
                title = tr("Modified");
                break;
            case ColumnFileCrTime:
                title = tr("Created");
                break;
            case ColumnFileDTime:
                title = tr("Deleted");
                break;
            case ColumnFileOwner:
                title = tr("Owner");
                break;
            case ColumnFileGroup:
                title = tr("Group");
                break;
            }
            return QVariant(title);
        }
    }
    return QVariant();
}

QModelIndex FolderModel::index(int row, int column, const QModelIndex& /*parent*/) const
{
    if(row < 0 || row >= mItems.size() || column < 0 || column >= NumOfColumns) {
        return QModelIndex();
    }
    const FolderModelItem& item = mItems.at(row);
    return createIndex(row, column, (void*)&item);
}

QModelIndex FolderModel::parent(const QModelIndex& /*index*/) const
{
    return QModelIndex();
}

Qt::ItemFlags FolderModel::flags(const QModelIndex& index) const
{
    // FIXME: should not return same flags unconditionally for all columns
    Qt::ItemFlags flags;
    if(index.isValid()) {
        flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        if(index.column() == ColumnFileName) {
            flags |= (Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable); // inline renaming);
        }
    }
    else {
        flags = Qt::ItemIsDropEnabled;
    }
    return flags;
}

std::shared_ptr<const FileInfo> FolderModel::fileInfoFromPath(const FilePath& path) const {
    QList<FolderModelItem>::const_iterator it = mItems.begin();
    while(it != mItems.end()) {
        const FolderModelItem& item = *it;
        if(item.mInfo->path() == path) {
            return item.mInfo;
        }
        ++it;
    }
    return nullptr;
}

// FIXME: this is very inefficient and should be replaced with a
// more reasonable implementation later.
QList<FolderModelItem>::iterator FolderModel::findItemByName(const char* name, int* row)
{
    QList<FolderModelItem>::iterator it = mItems.begin();
    int i = 0;
    while(it != mItems.end()) {
        FolderModelItem& item = *it;
        if(item.mInfo->name() == name) {
            *row = i;
            return it;
        }
        ++it;
        ++i;
    }
    return mItems.end();
}

QList< FolderModelItem >::iterator FolderModel::findItemByFileInfo(const FileInfo* info, int* row)
{
    QList<FolderModelItem>::iterator it = mItems.begin();
    int i = 0;
    while(it != mItems.end()) {
        FolderModelItem& item = *it;
        if(item.mInfo.get() == info) {
            *row = i;
            return it;
        }
        ++it;
        ++i;
    }
    return mItems.end();
}

QStringList FolderModel::mimeTypes() const
{
    //qDebug("FolderModel::mimeTypes");
    QStringList types = QAbstractItemModel::mimeTypes();
    // now types contains "application/x-qabstractitemmodeldatalist"

    // add support for freedesktop Xdnd direct save (XDS) protocol.
    // https://www.freedesktop.org/wiki/Specifications/XDS/#index4h2
    // the real implementation is in FolderView::childDropEvent().
    types << QStringLiteral("XdndDirectSave0");
    types << QStringLiteral("text/uri-list");
    types << QStringLiteral("libfm/files"); // see FolderModel::mimeData() below
    return types;
}

QMimeData* FolderModel::mimeData(const QModelIndexList& indexes) const
{
    QMimeData* data = QAbstractItemModel::mimeData(indexes);
    //qDebug("FolderModel::mimeData");
    // build two uri lists, one for internal DND
    // and the other for DNDing to external apps
    QByteArray urilist, libfmUrilist;
    urilist.reserve(4096);
    libfmUrilist.reserve(4096);

    for(const auto& index : indexes) {
        FolderModelItem* item = itemFromIndex(index);
        if(item && item->mInfo) {
            auto path = item->mInfo->path();
            if(path.isValid()) {
                // get the list that will be used by internal DND
                auto uri = path.uri();
                libfmUrilist.append(uri.get());
                libfmUrilist.append('\n');

                // also, get the list that will be used when DNDing to external apps,
                // using local paths as far as possible (for DNDing from remote folders)
                if(auto localPath = path.localPath()) {
                    QUrl url = QUrl::fromLocalFile(QString::fromUtf8(localPath.get()));
                    urilist.append(url.toEncoded());
                }
                else {
                    urilist.append(uri.get());
                }
                urilist.append('\n');
            }
        }
    }
    data->setData(QStringLiteral("text/uri-list"), urilist);
    // NOTE: The mimetype "text/uri-list" changes the list in QMimeData::setData() to get URLs
    // but some protocols (like MTP) may need the original list to query file info.
    data->setData(QStringLiteral("libfm/files"), libfmUrilist);

    return data;
}

bool FolderModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
    //qDebug("FolderModel::dropMimeData");
    if(!mFolder || !data) {
        return false;
    }
    FilePath destPath;
    if(parent.isValid()) { // drop on an item
        std::shared_ptr<const FileInfo> info;
        if(row == -1 && column == -1) {
            info = fileInfoFromIndex(parent);
        }
        else {
            QModelIndex itemIndex = index(row, column, parent);
            info = fileInfoFromIndex(itemIndex);
        }
        if(info) {
            if (info->isDir()) {
                destPath = info->path();
            }
            else {
                destPath = path(); // don't drop on file
            }
        }
        else {
            return false;
        }
    }
    else { // drop on blank area of the folder
        destPath = path();
    }

    FilePathList srcPaths;
    // try to get paths from the original data
    if(data->hasFormat(QStringLiteral("libfm/files"))) {
        QByteArray datat = data->data(QStringLiteral("libfm/files"));
        srcPaths = Utils::pathListFromUriList(datat.data());
    }
    if(srcPaths.empty() && data->hasUrls()) {
        srcPaths = Utils::pathListFromQUrls(data->urls());
    }

    // FIXME: should we put this in dropEvent handler of FolderView instead?
    if(!srcPaths.empty()) {
        //qDebug("drop action: %d", action);
        switch(action) {
        case Qt::CopyAction:
//            FileOperation::copyFiles(srcPaths, destPath);
            break;
        case Qt::MoveAction:
//            FileOperation::moveFiles(srcPaths, destPath);
            break;
        case Qt::LinkAction:
//            FileOperation::symlinkFiles(srcPaths, destPath);
        /* Falls through. */
        default:
            return false;
        }
        return true;
    }
    else if(data->hasFormat(QStringLiteral("application/x-qabstractitemmodeldatalist"))) {
        return true;
    }
    return QAbstractListModel::dropMimeData(data, action, row, column, parent);
}

Qt::DropActions FolderModel::supportedDropActions() const
{
    //qDebug("FolderModel::supportedDropActions");
    return Qt::CopyAction | Qt::MoveAction | Qt::LinkAction;
}

// ask the model to load thumbnails of the specified size
void FolderModel::cacheThumbnails(const int size)
{
    auto it = std::find_if(mThumbnailData.begin(), mThumbnailData.end(), [size](ThumbnailData& item) {return item.mSize == size;});
    if(it != mThumbnailData.cend()) {
        ++it->mRefCount;
    } else {
        mThumbnailData.push_front(ThumbnailData(size));
    }
}

// ask the model to free cached thumbnails of the specified size
void FolderModel::releaseThumbnails(int size)
{
    auto prev = mThumbnailData.before_begin();
    for(auto it = mThumbnailData.begin(); it != mThumbnailData.end(); ++it) {
        if(it->mSize == size) {
            --it->mRefCount;
            if(it->mRefCount == 0) {
                mThumbnailData.erase_after(prev);
            }

            // remove all cached thumbnails of the specified size
            QList<FolderModelItem>::iterator itemIt;
            for(itemIt = mItems.begin(); itemIt != mItems.end(); ++itemIt) {
                FolderModelItem& item = *itemIt;
                item.removeThumbnail(size);
            }
            break;
        }
        prev = it;
    }
}

void FolderModel::onThumbnailJobFinished()
{
    ThumbnailJob* job = static_cast<ThumbnailJob*>(sender());
    auto it = std::find(mPendingThumbnailJobs.cbegin(), mPendingThumbnailJobs.cend(), job);
    if(it != mPendingThumbnailJobs.end()) {
        mPendingThumbnailJobs.erase(it);
    }
}

void FolderModel::onThumbnailLoaded(const std::shared_ptr<const FileInfo>& file, int size, const QImage& image)
{
    // find the model item this thumbnail belongs to
    int row;
    QList<FolderModelItem>::iterator it = findItemByFileInfo(file.get(), &row);
    if(it != mItems.end()) {
        // the file is found in our model
        FolderModelItem& item = *it;
        QModelIndex index = createIndex(row, 0, (void*)&item);
        // store the image in the folder model item.
        FolderModelItem::Thumbnail* thumbnail = item.findThumbnail(size);
        thumbnail->image = image;
        // qDebug("thumbnail loaded for: %s, size: %d", item.displayName.toUtf8().constData(), size);
        if(image.isNull()) {
            thumbnail->status = FolderModelItem::ThumbnailFailed;
        } else {
            thumbnail->status = FolderModelItem::ThumbnailLoaded;
            thumbnail->image = image;

            // tell the world that we have the thumbnail loaded
            Q_EMIT thumbnailLoaded(index, size);
        }
    }
}

// get a thumbnail of size at the index
// if a thumbnail is not yet loaded, this will initiate loading of the thumbnail.
QImage FolderModel::thumbnailFromIndex(const QModelIndex& index, int size)
{
    FolderModelItem* item = itemFromIndex(index);
    if(item) {
        FolderModelItem::Thumbnail* thumbnail = item->findThumbnail(size);
        // qDebug("FolderModel::thumbnailFromIndex: %d, %s", thumbnail->status, item->displayName.toUtf8().data());
        switch(thumbnail->status) {
        case FolderModelItem::ThumbnailNotChecked: {
            // load the thumbnail
            queueLoadThumbnail(item->mInfo, size);
            thumbnail->status = FolderModelItem::ThumbnailLoading;
            break;
        }
        case FolderModelItem::ThumbnailLoaded:
            return thumbnail->image;
        default:
            ;
        }
    }
    return QImage();
}
}
