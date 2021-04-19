#include "testkey.h"

#include <QThread>
#include <QDebug>
#include <QObject>
#include <QApplication>
#include <QCoreApplication>
#include <QGSettings/QGSettings>

/**
 * 默认存放路径: /usr/share/glib-2.0/schemas
 * 选择 keep-window-open.gschema.xml测试
 *
 * schema id = "keep-window-open"
 */

int main (int argc, char* argv[])
{
    QApplication app(argc, argv);

    TestKey* k = new TestKey(nullptr);
    QGSettings* g = new QGSettings("org.gnome.Epiphany.ui");

    qDebug() << "keep-window-open:" << g->get("keep-window-open").toInt();

    qDebug()<<g->keys();

    qDebug() << QObject::connect(g, SIGNAL(changed(QString)), k, SLOT(testKey(QString)));

    return app.exec();
}
