#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLayout>

namespace Peony
{
class DirectoryViewContainer;
}

class SideBar;
class PathBar;
class ToolBar;
class NavigationBar;


class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

    QString getCurrentUri();

public Q_SLOTS:
    void cdUp();
    void goBack();
    void refresh();
    void goForward();
    void updateStatusBar();
    void updateWindowState();
    void switchView(const QString &viewId);
    void goToUri(const QString &uri, bool addToHistory = true);

private:
    SideBar*                            mSideBar;
    PathBar*                            mPathBar;
    ToolBar*                            mToolBar;
    NavigationBar*                      mNavigationBar;
    Peony::DirectoryViewContainer*      mContainer;
    QLayout*                            mMainContent;
};
#endif // DIALOG_H
