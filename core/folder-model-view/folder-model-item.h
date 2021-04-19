//
// Created by dingjing on 2021/3/30.
//

#ifndef LIBCORE_DINGJING_FOLDERMODELITEM_H
#define LIBCORE_DINGJING_FOLDERMODELITEM_H

#include <QIcon>
#include <QImage>
#include <QString>
#include <QVector>

#include "global.h"
#include "folder.h"

namespace dingjing
{
    class EXPORT_IMPORT_LIBRARY FolderModelItem
    {
    public:
        enum ThumbnailStatus
        {
            ThumbnailNotChecked,
            ThumbnailLoading,
            ThumbnailLoaded,
            ThumbnailFailed
        };

        struct Thumbnail
        {
            int             size;
            QImage          image;
            ThumbnailStatus status;
        };

    public:
        explicit FolderModelItem(const std::shared_ptr<const FileInfo>& _info);
        FolderModelItem(const FolderModelItem& other);
        virtual ~FolderModelItem();

        const std::string& name() const;
        const QString& displayName() const;

        QIcon icon() const;
        QString ownerName() const;
        QString ownerGroup() const;

        const QString& displaySize() const;
        const QString& displayDtime() const;
        const QString& displayMtime() const;
        const QString& displayCrtime() const;
        Thumbnail* findThumbnail(int size);

        void removeThumbnail(int size);

        bool                                mIsCut;
        mutable QString                     mDispMtime;
        mutable QString                     mDispCrtime;
        mutable QString                     mDispDtime;
        mutable QString                     mDispSize;
        QVector<Thumbnail>                  mThumbnails;
        std::shared_ptr<const FileInfo>     mInfo;
    };
}




#endif //LIBCORE_DINGJING_FOLDERMODELITEM_H
