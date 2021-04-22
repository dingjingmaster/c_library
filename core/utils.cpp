#include "utils.h"
#include "folder.h"

#include <QFile>
#include <QMessageBox>


bool dingjing::Utils::fileExists(const QString& file)
{
    QString filePath = file.split("://").last();

    return QFile::exists(filePath);
}

GFileType dingjing::Utils::getFileType(const QString &uri)
{
    GFileType fileType = G_FILE_TYPE_UNKNOWN;
    GFile* file = g_file_new_for_uri(uri.toUtf8().data());
    if (nullptr == file) {
        goto out;
    }

    fileType = g_file_query_file_type(file, G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, nullptr);

out:
    if (nullptr != file)        g_object_unref(file);

    return fileType;
}

dingjing::FilePathList dingjing::Utils::pathListFromQUrls(QList<QUrl> urls)
{
    FilePathList pathList;
    for(const auto & url : urls) {
        auto path = FilePath::fromUri(url.toString().toUtf8().constData());
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

QString dingjing::Utils::stringMiddleTruncate (const QString& str, uint truncateLength)
{
    glong                   length;
    glong                   numLeftChars;
    glong                   numRightChars;
    QString                 leftSubStr = nullptr;
    QString                 rightSubStr = nullptr;

    if (nullptr == str || truncateLength <= 0) {
        goto out;
    }

    length = str.length();
    if (length <= truncateLength) {
        goto out;
    }

    numLeftChars = truncateLength / 2;
    numRightChars = truncateLength - numLeftChars;

    if (numLeftChars <= 0 || numRightChars <= 0) {
        goto out;
    }

    leftSubStr = str.left(numLeftChars);
    rightSubStr = str.right(numRightChars);

    return QString("%1...%2").arg(leftSubStr).arg(rightSubStr);

out:
    return str;
}

bool dingjing::Utils::changeFileName(const dingjing::FilePath &filePath, const QString &newName, QWidget *parent, bool showMessage)
{
    GErrorPtr err;
    GFilePtr gfile{g_file_set_display_name(filePath.gfile().get(),
                                            newName.toLocal8Bit().constData(),
                                            nullptr, /* make this cancellable later. */
                                            &err)};
    if(gfile == nullptr) {
        if (showMessage){
            QMessageBox::critical(parent ? parent->window() : nullptr, QObject::tr("Error"), err.message());
        }
        return false;
    }

    // reload the containing folder if it is in use but does not have a file monitor
    auto folder = Folder::findByPath(filePath.parent());
    if(folder && folder->isValid() && folder->isLoaded() && !folder->hasFileMonitor()) {
        folder->reload();
    }

    return true;
}
