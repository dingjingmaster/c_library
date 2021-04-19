#ifndef PATHBAR_H
#define PATHBAR_H

#include <QWidget>

#include "file-path.h"

class QToolButton;
class QScrollArea;
class QPushButton;
class QHBoxLayout;

namespace dingjing
{

class PathEditor;
class PathButton;

class PathBar: public QWidget
{
    Q_OBJECT
public:
    explicit PathBar(QWidget* parent = nullptr);

    const FilePath& path()
    {
        return mCurrentPath;
    }

    void setPath(FilePath path);

Q_SIGNALS:
    void chdir(const FilePath& path);
    void middleClickChdir(const FilePath& path);
    void editingFinished();

public Q_SLOTS:
    void openEditor();
    void closeEditor();
    void copyPath();

private Q_SLOTS:
  void onButtonToggled(bool checked);
  void onScrollButtonClicked();
  void onReturnPressed();
  void setArrowEnabledState(int value);
  void setScrollButtonVisibility();
  void ensureToggledVisible();

protected:
    void wheelEvent(QWheelEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

private:
    void updateScrollButtonVisibility();
    FilePath pathForButton(PathButton* btn);

private:
    QWidget*            mButtonsWidget;
    QToolButton*        mScrollToStart;
    QToolButton*        mScrollToEnd;
    QScrollArea*        mScrollArea;
    QHBoxLayout*        mButtonsLayout;
    PathEditor*         mTempPathEdit;

    FilePath            mCurrentPath;   // currently active path
    PathButton*         mToggledBtn;
};

}

#endif // PATHBAR_H
