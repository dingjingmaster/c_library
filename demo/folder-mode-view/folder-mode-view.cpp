#include <QDebug>
#include <QToolBar>
#include <QMainWindow>
#include <QApplication>

#include "core/folder.h"
#include "folder-model-view/folder-view.h"
#include "folder-model-view/folder-model.h"
#include "folder-model-view/proxy-folder-model.h"
#include <folder-model-view/cached-folder-model.h>

using namespace dingjing;

int main (int argc, char* argv[])
{
    QApplication app(argc, argv);

    QMainWindow win;

    FolderView folderView;
    win.setCentralWidget(&folderView);

    auto home = FilePath::homeDir();
    CachedFolderModel* model = CachedFolderModel::modelFromPath(home);

    auto proxyModel = new ProxyFolderModel();
    proxyModel->sort(FolderModel::ColumnFileName, Qt::AscendingOrder);
    proxyModel->setSourceModel(model);

    proxyModel->setThumbnailSize(64);
    proxyModel->setShowThumbnails(true);

    folderView.setModel(proxyModel);

    QToolBar toolbar;
    win.addToolBar(Qt::TopToolBarArea, &toolbar);

    win.show();

    return app.exec();
}
