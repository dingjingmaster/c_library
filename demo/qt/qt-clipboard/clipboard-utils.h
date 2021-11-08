#ifndef CLIPBOARDUTILS_H
#define CLIPBOARDUTILS_H

#include <QObject>
#include <QWidget>

class ClipboardUtils : public QWidget
{
    Q_OBJECT

public:
    explicit ClipboardUtils (QWidget *parent = nullptr);

protected:
    bool eventFilter (QObject* obj, QEvent* ev) override;

signals:

};

#endif // CLIPBOARDUTILS_H
