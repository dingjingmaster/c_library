#ifndef CLIPBOARDUTILS_H
#define CLIPBOARDUTILS_H

#include <QObject>

class ClipboardUtils : public QObject
{
    Q_OBJECT

public:
    explicit ClipboardUtils (QObject *parent = nullptr);

protected:
    bool eventFilter (QObject* obj, QEvent* ev) override;

signals:

};

#endif // CLIPBOARDUTILS_H
