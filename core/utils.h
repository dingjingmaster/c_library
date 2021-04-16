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
    static FilePathList pathListFromQUrls (QList<QUrl> urls);
    static FilePathList pathListFromUriList (const char* uriList);
    static QByteArray pathListToUriList (const FilePathList& paths);

    static bool changeFileName(const FilePath& filePath, const QString& newName, QWidget* parent, bool showMessage=true);
};
}

#endif // UTILS_H
