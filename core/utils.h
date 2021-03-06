#ifndef UTILS_H
#define UTILS_H

#include <QUrl>
#include <QList>
#include <cstdint>
#include <utility>
#include <QMimeData>
#include <sys/types.h>

#include "global.h"
#include "file-path.h"
#include "file-info.h"

namespace dingjing
{
class Utils
{
public:
    static bool fileExists (const QString& file);
    static GFileType getFileType(const QString& uri);
    static FilePathList pathListFromQUrls (QList<QUrl> urls);
    static FilePathList pathListFromUriList (const char* uriList);
    static QByteArray pathListToUriList (const FilePathList& paths);

    static QString stringMiddleTruncate (const QString& str, uint truncateLength);

    static bool changeFileName(const FilePath& filePath, const QString& newName, QWidget* parent, bool showMessage=true);
};
}

#endif // UTILS_H
