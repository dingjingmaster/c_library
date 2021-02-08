#include "dialog.h"

#include "path-bar.h"
#include "tool-bar.h"
#include "side-bar.h"
#include "navigation-bar.h"

#include <QUrl>
#include <QDockWidget>
#include <PeonyFileInfo>
#include <QDesktopServices>
#include <QLayout>

#include <peony-qt/controls/directory-view/directory-view-widget.h>
#include <peony-qt/controls/directory-view/directory-view-container.h>
#include <peony-qt/controls/menu/directory-view-menu/directory-view-menu.h>

Dialog::Dialog(QWidget *parent) : QDialog(parent)
{
    new QGridLayout(this);
    layout()->setSpacing(0);
    layout()->setContentsMargins(0, 0, 0, 0);

    mContainer = new Peony::DirectoryViewContainer;
    mContainer->switchViewType("Icon View");
    mContainer->goToUri("file:///", false);

    mNavigationBar = new NavigationBar;
    mMainContent = new QHBoxLayout;
    mSideBar = new SideBar;

    mSideBar->setFixedWidth(180);
    mNavigationBar->setFixedHeight(50);

    mMainContent->addWidget(mSideBar);
    mMainContent->addWidget(mContainer);
    mContainer->setContentsMargins(0, 0, 0, 0);

    qobject_cast<QGridLayout*>(layout())->addWidget(mNavigationBar);
    qobject_cast<QGridLayout*>(layout())->addLayout(mMainContent, 1, 0);

//     layout()->
//    layout()->addItem(mMainContent);

}

Dialog::~Dialog()
{
}

void Dialog::cdUp()
{

}

void Dialog::goBack()
{

}

void Dialog::refresh()
{

}

void Dialog::goForward()
{

}

void Dialog::updateStatusBar()
{

}

void Dialog::updateWindowState()
{

}

void Dialog::switchView(const QString &viewId)
{

}

void Dialog::goToUri(const QString &uri, bool addToHistory)
{

}

