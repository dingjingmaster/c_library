#ifndef CACHEDFOLDERMODEL_H
#define CACHEDFOLDERMODEL_H

#include "folder.h"
#include "folder-model-view/folder-model.h"

namespace dingjing
{
class CachedFolderModel : public FolderModel {
    Q_OBJECT
public:
    explicit CachedFolderModel(const std::shared_ptr<Folder>& folder);
    void ref()
    {
        ++mRefCount;
    }
    void unref();

    static CachedFolderModel* modelFromPath(const FilePath& path);
    static CachedFolderModel* modelFromFolder(const std::shared_ptr<Folder>& folder);

private:
    ~CachedFolderModel() override;

private:
    int                             mRefCount;
    constexpr static const char*    gCacheKey = "CachedFolderModel";
};
}

#endif // CACHEDFOLDERMODEL_H
