#include "clipboard-utils.h"

#include <QDebug>
#include <QApplication>
#include <QClipboard>
#include <QMetaObject>
#include <QMetaProperty>

ClipboardUtils::ClipboardUtils(QObject *parent) : QObject(parent)
{
    qApp->clipboard ()->installEventFilter (this);

    connect (QApplication::clipboard (), &QClipboard::changed, this, [=] () {
        qDebug() << QApplication::clipboard ()->text ();
        const QMetaObject* metaObject = QApplication::clipboard ()->metaObject();

        for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i) {
            qDebug() << "static property name ===>" << QString::fromLatin1(metaObject->property(i).name());
        }

        QList<QByteArray> dp = QApplication::clipboard ()->dynamicPropertyNames();
        for (auto s = dp.constBegin (); s != dp.constEnd (); ++s) {
            qDebug() << "dynamicPropertyNames ===>" << *s;
        }

        qDebug() << "=================================================";
    });
}

bool ClipboardUtils::eventFilter(QObject *obj, QEvent *ev)
{
    qDebug() << ev;

    return false;

    Q_UNUSED (obj);
}
