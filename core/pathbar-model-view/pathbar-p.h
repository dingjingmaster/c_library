#ifndef PATHBARP_H
#define PATHBARP_H

#include <string>
#include <QStyle>
#include <QEvent>
#include <QString>
#include <QToolButton>
#include <QMouseEvent>

namespace dingjing
{

class PathButton: public QToolButton
{
    Q_OBJECT
public:
    PathButton(std::string name, QString displayName, bool isRoot = false, QWidget* parent = nullptr) : QToolButton(parent), mName{name}
    {
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
        setCheckable(true);
        setAutoExclusive(true);
        setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        /* respect the toolbar icon size (can be set with some styles) */
        int icnSize = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
        setIconSize(QSize(icnSize, icnSize));

        setText(displayName.replace(QLatin1Char('\n'), QLatin1Char(' '))); // single-line text

        if(isRoot) { /* this element is root */
            QIcon icon = QIcon::fromTheme(QStringLiteral("drive-harddisk"));
            setIcon(icon);
        }
    }

    void changeEvent(QEvent* event) override
    {
        QToolButton::changeEvent(event);
        if(event->type() == QEvent::StyleChange) {
            int icnSize = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
            setIconSize(QSize(icnSize, icnSize));
        }
    }

    std::string name() const
    {
        return mName;
    }

    void setName(const std::string& name)
    {
        mName = name;
    }

private:
    std::string         mName;
};

}

#endif // PATHBARP_H
