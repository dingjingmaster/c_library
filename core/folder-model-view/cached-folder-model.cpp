#include "cached-folder-model.h"

namespace dingjing
{
CachedFolderModel::CachedFolderModel(const std::shared_ptr<Folder>& folder):
    FolderModel(),
    mRefCount(1)
{
    FolderModel::setFolder(folder);
}

CachedFolderModel::~CachedFolderModel()
{
    // qDebug("delete CachedFolderModel");
}

CachedFolderModel* CachedFolderModel::modelFromFolder(const std::shared_ptr<Folder>& folder)
{
    QVariant cache = folder->property(gCacheKey);
    CachedFolderModel* model = cache.value<CachedFolderModel*>();
    if(model) {
        model->ref();
    }
    else {
        model = new CachedFolderModel(folder);
        cache = QVariant::fromValue(model);
        folder->setProperty(gCacheKey, cache);
    }
    return model;
}

CachedFolderModel* CachedFolderModel::modelFromPath(const FilePath& path)
{
    auto folder = Folder::fromPath(path);
    if(folder) {
        CachedFolderModel* model = modelFromFolder(folder);
        return model;
    }
    return nullptr;
}

void CachedFolderModel::unref()
{
    // qDebug("unref cache");
    --mRefCount;
    if(mRefCount <= 0) {
        folder()->setProperty(gCacheKey, QVariant());
        delete(this);
    }
}


}
