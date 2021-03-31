#ifndef FOLDERITEMDELEGATE_H
#define FOLDERITEMDELEGATE_H

#include "core/global.h"

#include <QStyledItemDelegate>

class QAbstractItemView;

namespace dingjing
{
class FolderItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit FolderItemDelegate(QAbstractItemView* view, QObject* parent = nullptr);

    ~FolderItemDelegate() override;

    inline void setItemSize(QSize size)
    {
        mItemSize = size;
    }

    inline QSize itemSize() const
    {
        return mItemSize;
    }

    inline void setIconSize(QSize size)
    {
        mIconSize = size;
    }

    inline QSize iconSize() const
    {
        return mIconSize;
    }

    int fileInfoRole()
    {
        return mFileInfoRole;
    }

    void setFileInfoRole(int role)
    {
        mFileInfoRole = role;
    }

    int iconInfoRole()
    {
        return mIconInfoRole;
    }

    void setIconInfoRole(int role)
    {
        mIconInfoRole = role;
    }

    // only support vertical layout (icon view mode: text below icon)
    void setShadowColor(const QColor& shadowColor)
    {
      mShadowColor = shadowColor;
    }

    // only support vertical layout (icon view mode: text below icon)
    const QColor& shadowColor() const
    {
      return mShadowColor;
    }

    // only support vertical layout (icon view mode: text below icon)
    void setMargins(QSize margins)
    {
      mMargins = margins.expandedTo(QSize(0, 0));
    }

    QSize getMargins() const
    {
      return mMargins;
    }

    bool hasEditor() const
    {
        return mHasEditor;
    }

    void setShadowHidden(bool value)
    {
        mShadowHidden = value;
    }

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    void setEditorData(QWidget* editor, const QModelIndex& index) const override;

    bool eventFilter(QObject* object, QEvent* event) override;

    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    QSize iconViewTextSize(const QModelIndex& index) const;

private:
    void drawText(QPainter* painter, QStyleOptionViewItem& opt, QRectF& textRect) const;

    static QIcon::Mode iconModeFromState(QStyle::State state);

private:
    QIcon           mSymlinkIcon;
    QIcon           mUntrustedIcon;
    QIcon           mAddIcon;
    QIcon           mRemoveIcon;
    QSize           mIconSize;
    QSize           mItemSize;
    int             mFileInfoRole;
    int             mIconInfoRole;
    QColor          mShadowColor;
    QSize           mMargins;
    bool            mShadowHidden;
    mutable bool    mHasEditor;
};
}


#endif // FOLDERITEMDELEGATE_H
