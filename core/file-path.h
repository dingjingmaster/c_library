//
// Created by dingjing on 2021/3/30.
//

#ifndef LIBCORE_DINGJING_FILE_PATH_H
#define LIBCORE_DINGJING_FILE_PATH_H

#include "global.h"
#include "smart-ptr.hpp"

#include <glib.h>
#include <vector>
#include <gio/gio.h>

#include <QObject>

namespace dingjing
{
class FilePath
{
public:
    explicit FilePath();
    explicit FilePath(GFile* gfile, bool add_ref);

    FilePath(const FilePath& other);
    FilePath(FilePath&& other) noexcept;
    static FilePath fromUri(const char* uri);
    static FilePath fromLocalPath(const char* path);
    static FilePath fromPathStr(const char* pathStr);
    static FilePath fromDisplayName(const char* path);

    CStrPtr uri() const;
    bool isValid() const;
    CStrPtr baseName() const;
    CStrPtr toString() const;
    unsigned int hash() const;
    CStrPtr localPath() const;
    CStrPtr displayName() const;            // a human readable UTF-8 display name for the path

    bool hasParent() const;
    FilePath parent() const;

    bool isParentOf(const FilePath& other) const;
    bool isPrefixOf(const FilePath& other) const;

    FilePath child(const char* name) const;
    FilePath relativePath(const char* relPath) const;
    CStrPtr relativePathStr(const FilePath& descendant) const;

    bool isNative() const;
    CStrPtr uriScheme() const;
    const GObjectPtr<GFile>& gfile() const;
    bool hasUriScheme(const char* scheme) const;

    operator bool() const;
    bool operator != (std::nullptr_t) const;
    bool operator == (GFile* other_gfile) const;
    bool operator == (const FilePath& other) const;
    bool operator != (const FilePath& other) const;
    FilePath& operator = (const FilePath& other);
    FilePath& operator = (const FilePath&& other) noexcept;

    static const FilePath& homeDir();

private:
    GObjectPtr<GFile>           mGfile;
    static FilePath             mHomeDir;
};

struct FilePathHash
{
    std::size_t operator() (const FilePath& path) const {
        return path.hash();
    }
};

typedef std::vector<FilePath> FilePathList;
};

Q_DECLARE_METATYPE(dingjing::FilePath)

#endif //LIBCORE_DINGJING_FILE_PATH_H
