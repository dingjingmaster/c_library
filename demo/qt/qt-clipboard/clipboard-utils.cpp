#include "clipboard-utils.h"

#include <QDebug>
#include <unistd.h>
#include <QApplication>
#include <QClipboard>
#include <QMetaObject>
#include <QMetaProperty>

ClipboardUtils::ClipboardUtils(QWidget *parent) : QWidget (parent)
{
    qApp->clipboard ()->installEventFilter (this);

    connect (QApplication::clipboard (), &QClipboard::changed, this, [=] () {
        qDebug() << "0:" << QApplication::clipboard ()->text ();
        const QMetaObject* metaObject = QApplication::clipboard ()->metaObject();

        for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i) {
            qDebug() << "static property name ===>" << QString::fromLatin1(metaObject->property(i).name());
        }

        QList<QByteArray> dp = QApplication::clipboard ()->dynamicPropertyNames();
        for (auto s = dp.constBegin (); s != dp.constEnd (); ++s) {
            qDebug() << "dynamicPropertyNames ===>" << *s;
        }

        bool flag = true;
        while (flag) {
            sleep (2);
            QString s = QApplication::clipboard ()->text ();
            if (!s.isEmpty ()) flag = false;
            qDebug() << "0:" << s;
        }

        qDebug() << "=================================================";
    });

    connect (QApplication::clipboard (), &QClipboard::dataChanged, this, [=] () {
        qDebug () << "1:" << QApplication::clipboard ()->text ();
    });

    connect (QApplication::clipboard (), &QClipboard::findBufferChanged, this, [=] () {
        qDebug () << "2:" << QApplication::clipboard ()->text ();
    });

    connect (QApplication::clipboard (), &QClipboard::selectionChanged, this, [=] () {
        qDebug () << "3:" << QApplication::clipboard ()->text ();
    });
}

bool ClipboardUtils::eventFilter(QObject *obj, QEvent *ev)
{
    qDebug() << ev;

    return false;

    Q_UNUSED (obj);
}
