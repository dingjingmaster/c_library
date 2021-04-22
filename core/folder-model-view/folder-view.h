#ifndef FOLDEDRVIEW_H
#define FOLDEDRVIEW_H
#include <QWidget>
#include <QListView>
#include <QTreeView>
#include <QMouseEvent>

#include "folder.h"
#include "folder-model.h"
#include "proxy-folder-model.h"

class QTimer;

namespace dingjing
{

class FileMenu;
class FolderMenu;
class FileLauncher;
class FolderViewStyle;

class FolderView : public QWidget
{
    Q_OBJECT
public:
    enum ViewMode
    {
        FirstViewMode = 1,
        IconMode = FirstViewMode,
        CompactMode,
        DetailedListMode,
        ThumbnailMode,
        LastViewMode = ThumbnailMode,
        NumViewModes = (LastViewMode - FirstViewMode + 1)
    };

    enum ClickType
    {
        ActivatedClick,
        MiddleClick,
        ContextMenuClick
    };

    friend class FolderViewTreeView;
    friend class FolderViewListView;

    explicit FolderView(ViewMode mode = IconMode, QWidget* parent = nullptr);

    explicit FolderView(QWidget* parent): FolderView{IconMode, parent} {}

    ~FolderView() override;

    void setViewMode(ViewMode mode);
    ViewMode viewMode() const;

    void setIconSize(ViewMode mode, QSize size);
    QSize iconSize(ViewMode mode) const;

    QAbstractItemView* childView() const;

    ProxyFolderModel* model() const;
    void setModel(ProxyFolderModel* model);

    std::shared_ptr<Folder> folder() const
    {
        return mModel ? static_cast<FolderModel*>(mModel->sourceModel())->folder() : nullptr;
    }

    std::shared_ptr<const FileInfo> folderInfo() const
    {
        auto mFolder = folder();
        return mFolder ? mFolder->info() : nullptr;
    }

    FilePath path()
    {
        auto mFolder = folder();
        return mFolder ? mFolder->path() : FilePath();
    }

    QItemSelectionModel* selectionModel() const;
    FileInfoList selectedFiles() const;
    FilePathList selectedFilePaths() const;
    bool hasSelection() const;
    QModelIndex indexFromFolderPath(const FilePath& folderPath) const;
    void selectFiles(const FileInfoList& files, bool add = false);

    void selectAll();

    void invertSelection();

    void setFileLauncher(FileLauncher* launcher)
    {
        mFileLauncher = launcher;
    }

    FileLauncher* fileLauncher()
    {
        return mFileLauncher;
    }

    int autoSelectionDelay() const
    {
        return mAutoSelectionDelay;
    }

    void setAutoSelectionDelay(int delay);

    void setShadowHidden(bool shadowHidden);

    void setCtrlRightClick(bool ctrlRightClick);

    QList<int> getCustomColumnWidths() const {
        return mCustomColumnWidths;
    }
    void setCustomColumnWidths(const QList<int> &widths);

    QList<int> getHiddenColumns() const {
        return mHiddenColumns.values();
    }
    void setHiddenColumns(const QList<int> &columns);

protected:
    bool event(QEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;
    virtual void childMousePressEvent(QMouseEvent* event);
    virtual void childDragEnterEvent(QDragEnterEvent* event);
    virtual void childDragMoveEvent(QDragMoveEvent* e);
    virtual void childDragLeaveEvent(QDragLeaveEvent* e);
    virtual void childDropEvent(QDropEvent* e);

    void emitClickedAt(ClickType type, const QPoint& pos);

    QModelIndexList selectedRows(int column = 0) const;
    QModelIndexList selectedIndexes() const;

    virtual void prepareFileMenu(FileMenu* menu);
    virtual void prepareFolderMenu(FolderMenu* menu);

    bool eventFilter(QObject* watched, QEvent* event) override;

    void updateGridSize(); // called when view mode, icon size, font size or cell margin is changed

    QSize getMargins() const
    {
        return mItemDelegateMargins;
    }

    // sets the cell margins in the icon and thumbnail modes
    // and calls updateGridSize() when needed
    void setMargins(QSize size);

public Q_SLOTS:
    void onItemActivated(QModelIndex index);
    virtual void onFileClicked(int type, const std::shared_ptr<const FileInfo>& fileInfo);
    void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

private Q_SLOTS:
    void scrollSmoothly();
    void onSelChangedTimeout();
    void onAutoSelectionTimeout();
    void onClosingEditor(QWidget* editor, QAbstractItemDelegate::EndEditHint hint);

Q_SIGNALS:
    void clicked(int type, const std::shared_ptr<const FileInfo>& file);
    void selChanged();
    void sortChanged();
    void clickedBack();
    void clickedForward();

    void columnHiddenByUser();
    void columnResizedByUser();

private:
    QAbstractItemView*      mView;
    ProxyFolderModel*       mModel;
    ViewMode                mMode;
    QSize                   mIconSize[NumViewModes];
    FileLauncher*           mFileLauncher;
    int                     mAutoSelectionDelay;
    QTimer*                 mAutoSelectionTimer;
    QModelIndex             mLastAutoSelectionIndex;
    QTimer*                 mSelChangedTimer;

    QSize                   mItemDelegateMargins;
    bool                    mShadowHidden;
    bool                    mCtrlRightClick; // show folder context menu with Ctrl + right click

    // smooth scrolling:
    struct scrollData
    {
        int delta;
        int leftFrames;
    };

    QSet<int>               mHiddenColumns;
    QList<int>              mCustomColumnWidths;

    QTimer*                 mSmoothScrollTimer;
    QList<scrollData>       mQueuedScrollSteps;
};

}



#endif // FOLDEDRVIEW_H
