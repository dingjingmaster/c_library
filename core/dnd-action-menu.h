#ifndef DNDACTIONMENU_H
#define DNDACTIONMENU_H

#include <QMenu>
#include <QObject>
#include <QAction>

namespace dingjing
{
class DndActionMenu : public QMenu
{
    Q_OBJECT
public:
    explicit DndActionMenu(Qt::DropActions possibleActions, QWidget* parent = nullptr);
    ~DndActionMenu() override;

    static Qt::DropAction askUser(Qt::DropActions possibleActions, QPoint pos);

private:
    QAction*        mCopyAction;
    QAction*        mMoveAction;
    QAction*        mLinkAction;
    QAction*        mCancelAction;
};
}



#endif // DNDACTIONMENU_H
