#include "dnd-action-menu.h"

namespace dingjing
{
DndActionMenu::DndActionMenu(Qt::DropActions possibleActions, QWidget* parent)
    : QMenu(parent)
    , mCopyAction(nullptr)
    , mMoveAction(nullptr)
    , mLinkAction(nullptr)
    , mCancelAction(nullptr)
{
    if(possibleActions.testFlag(Qt::CopyAction)) {
        mCopyAction = addAction(QIcon::fromTheme(QStringLiteral("edit-copy")), tr("Copy here"));
    }
    if(possibleActions.testFlag(Qt::MoveAction)) {
        mMoveAction = addAction(tr("Move here"));
    }
    if(possibleActions.testFlag(Qt::LinkAction)) {
        mLinkAction = addAction(tr("Create symlink here"));
    }
    addSeparator();
    mCancelAction = addAction(tr("Cancel"));
}

DndActionMenu::~DndActionMenu()
{

}

Qt::DropAction DndActionMenu::askUser(Qt::DropActions possibleActions, QPoint pos)
{
    Qt::DropAction result = Qt::IgnoreAction;
    DndActionMenu menu{possibleActions};
    QAction* action = menu.exec(pos);
    if(nullptr != action) {
        if(action == menu.mCopyAction) {
            result = Qt::CopyAction;
        } else if(action == menu.mMoveAction) {
            result = Qt::MoveAction;
        } else if(action == menu.mLinkAction) {
            result = Qt::LinkAction;
        }
    }
    return result;
}

}
