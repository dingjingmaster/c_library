//
// Created by dingjing on 2021/3/30.
//

#include "file-path.h"
#include <utility>

namespace dingjing
{

FilePath FilePath::mHomeDir;

dingjing::FilePath::operator bool() const
{
    return mGfile != nullptr;
}

FilePath::FilePath()
{

}

FilePath::FilePath(GFile *gfile, bool add_ref): mGfile{gfile, add_ref}
{

}

FilePath::FilePath(const FilePath &other): FilePath{}
{
    *this = other;
}

FilePath::FilePath(FilePath &&other) noexcept: FilePath{}
{
    *this = other;
}

FilePath FilePath::fromUri(const char *uri)
{
    return FilePath{g_file_new_for_uri(uri), false};
}

FilePath FilePath::fromLocalPath(const char *path)
{
    return FilePath {g_file_new_for_path(path), false};
}

FilePath FilePath::fromDisplayName(const char *path)
{
    return FilePath{g_file_parse_name(path), false};
}

FilePath FilePath::fromPathStr(const char *pathStr)
{
    return FilePath{g_file_new_for_commandline_arg(pathStr), false};
}

bool FilePath::isValid() const
{
    return mGfile != nullptr;
}

unsigned int FilePath::hash() const
{
    return g_file_hash(mGfile.get());
}

CStrPtr FilePath::baseName() const
{
    return CStrPtr{g_file_get_basename(mGfile.get())};
}

CStrPtr FilePath::localPath() const
{
    return CStrPtr{g_file_get_path(mGfile.get())};
}

CStrPtr FilePath::uri() const
{
    return CStrPtr{g_file_get_uri(mGfile.get())};
}

CStrPtr FilePath::toString() const
{
    if(isNative()) {
        return localPath();
    }

    return uri();
}

CStrPtr FilePath::displayName() const
{
    return CStrPtr{g_file_get_parse_name(mGfile.get())};
}

FilePath FilePath::parent() const
{
    return FilePath{g_file_get_parent(mGfile.get()), false};
}

bool FilePath::hasParent() const
{
    return g_file_has_parent(mGfile.get(), nullptr);
}

bool FilePath::isParentOf(const FilePath &other) const
{
    return g_file_has_parent(other.mGfile.get(), mGfile.get());
}

bool FilePath::isPrefixOf(const FilePath &other) const
{
    return g_file_has_prefix(other.mGfile.get(), mGfile.get());
}

FilePath FilePath::child(const char *name) const
{
    return FilePath{g_file_get_child(mGfile.get(), name), false};
}

CStrPtr FilePath::relativePathStr(const FilePath &descendant) const
{
    return CStrPtr{g_file_get_relative_path(mGfile.get(), descendant.mGfile.get())};
}

FilePath FilePath::relativePath(const char *relPath) const
{
    return FilePath{g_file_resolve_relative_path(mGfile.get(), relPath), false};
}

bool FilePath::isNative() const
{
    return g_file_is_native(mGfile.get());
}

bool FilePath::hasUriScheme(const char *scheme) const
{
    return g_file_has_uri_scheme(mGfile.get(), scheme);
}

CStrPtr FilePath::uriScheme() const
{
    return CStrPtr{g_file_get_uri_scheme(mGfile.get())};
}

const GObjectPtr<GFile> &FilePath::gfile() const
{
    return mGfile;
}

FilePath &FilePath::operator =(const FilePath &other)
{
    mGfile = other.mGfile;
    return *this;
}

FilePath &FilePath::operator =(const FilePath &&other) noexcept
{
    mGfile = std::move(other.mGfile);
    return *this;
}

bool FilePath::operator ==(const FilePath &other) const
{
    return operator==(other.mGfile.get());
}

bool FilePath::operator ==(GFile *other_gfile) const
{
    if(mGfile == other_gfile) {
        return true;
    }
    if(mGfile && other_gfile) {
        return g_file_equal(mGfile.get(), other_gfile);
    }
    return false;
}

bool FilePath::operator !=(const FilePath &other) const
{
    return !operator==(other);
}

bool FilePath::operator !=(std::nullptr_t) const
{
    return mGfile != nullptr;
}

const FilePath &FilePath::homeDir()
{
    if(!mHomeDir) {
        const char* home = getenv("HOME");
        if(!home) {
            home = g_get_home_dir();
        }
        mHomeDir = FilePath::fromLocalPath(home);
    }

    return mHomeDir;
}
}
