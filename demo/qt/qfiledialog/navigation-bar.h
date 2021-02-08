#ifndef NAVIGATIONBAR_H
#define NAVIGATIONBAR_H

#include <QToolBar>

class QAction;

class NavigationBar : public QToolBar
{
    Q_OBJECT
public:
    explicit NavigationBar(QWidget *parent = nullptr);

Q_SIGNALS:
    void goBackRequest();
    void goForwardRequest();
    void cdUpRequest();
    void refreshRequest();

public Q_SLOTS:
    void setCanGoBack(bool can);
    void setCanGoForward(bool can);
    void setCanCdUp(bool can);

private:
    QAction*            mGoBack;
    QAction*            mGoForward;
    QAction*            mCdUp;
    QAction*            mRefresh;
};
#endif // NAVIGATIONBAR_H
