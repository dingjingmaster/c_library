#include "utils.h"


dingjing::FilePathList dingjing::Utils::pathListFromQUrls(QList<QUrl> urls)
{
    FilePathList pathList;
    for(auto it = urls.cbegin(); it != urls.cend(); ++it) {
        auto path = FilePath::fromUri(it->toString().toUtf8().constData());
        pathList.push_back(std::move(path));
    }

    return pathList;
}

dingjing::FilePathList dingjing::Utils::pathListFromUriList(const char *uriList)
{
    FilePathList pathList;
    char** urist = g_strsplit_set(uriList, "\r\n", -1);
    for(char** uri = urist; *uri; ++uri) {
        if(**uri != '\0') {
            pathList.push_back(FilePath::fromUri(*uri));
        }
    }
    g_strfreev(urist);

    return pathList;
}

QByteArray dingjing::Utils::pathListToUriList(const dingjing::FilePathList &paths)
{
    QByteArray uriList;
    for(auto& path: paths) {
        uriList += path.uri().get();
        uriList += "\r\n";
    }

    return uriList;
}
