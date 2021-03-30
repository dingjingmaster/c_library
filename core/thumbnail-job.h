#ifndef THUMBNAILJOB_H
#define THUMBNAILJOB_H

#include "job.h"
#include "global.h"
#include "file-info.h"
#include "smart-ptr.hpp"

#include <QThreadPool>


namespace dingjing
{

class EXPORT_IMPORT_LIBRARY ThumbnailJob : public Job
{
    Q_OBJECT
public:
    explicit ThumbnailJob(FileInfoList files, int size);
    ~ThumbnailJob () override;

    int size () const;
    const std::vector<QImage>& results () const;

    static bool localFilesOnly ();
    static QThreadPool* threadPool();
    static int maxThumbnailFileSize ();
    static void setLocalFilesOnly(bool value);
    static int maxExternalThumbnailFileSize ();
    static void setMaxThumbnailFileSize (int size);
    static void setMaxExternalThumbnailFileSize (int size);

Q_SIGNALS:
    void thumbnailLoaded(const std::shared_ptr<const FileInfo>& file, int size, QImage thumbnail);

protected:
    void exec() override;

private:
    QImage readImageFromStream (GInputStream* stream, size_t len);
    QImage loadForFile (const std::shared_ptr<const FileInfo>& file);
    bool readJpegExif (GInputStream* stream, QImage& thumbnail, QTransform& matrix);
    bool isSupportedImageType (const std::shared_ptr<const MimeType>& mimeType) const;
    bool isThumbnailOutdated (const std::shared_ptr<const FileInfo>& file, const QImage& thumbnail) const;
    QImage generateThumbnail (const std::shared_ptr<const FileInfo>& file, const FilePath& origPath, const char* uri, const QString& thumbnailFilename);

private:
    static bool                 mLocalFilesOnly;
    static int                  mMaxThumbnailFileSize;
    static int                  mMaxExternalThumbnailFileSize;
    static QThreadPool*         mThreadPool;

    int                         mSize;
    FileInfoList                mFiles;
    GChecksum*                  mMd5Calc;
    std::vector<QImage>         mResults;
    GCancellablePtr             mCancellable;
};

}



#endif // THUMBNAILJOB_H
