#include "navigation-bar.h"

NavigationBar::NavigationBar(QWidget *parent) : QToolBar(parent)
{
    mGoBack = addAction("Go Back", this, &NavigationBar::goBackRequest);
    mGoForward = addAction("Go Forward", this, &NavigationBar::goForwardRequest);
    mCdUp = addAction("Cd Up", this, &NavigationBar::cdUpRequest);
    mRefresh = addAction("Refresh", this, &NavigationBar::refreshRequest);

    setCanGoBack(false);
    setCanGoForward(false);
    setCanCdUp(false);
}

void NavigationBar::setCanGoBack(bool can)
{
    mGoBack->setEnabled(can);
}

void NavigationBar::setCanGoForward(bool can)
{
    mGoForward->setEnabled(can);
}

void NavigationBar::setCanCdUp(bool can)
{
    mCdUp->setEnabled(can);
}
