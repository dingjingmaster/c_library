#include <QApplication>

#include <QDebug>
#include <QMimeData>
#include <QClipboard>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QClipboard* cp = qApp->clipboard();

    cp->connect(cp, &QClipboard::dataChanged, [=]() {
        qDebug() << QApplication::clipboard()->mimeData()->text();

    });

    qDebug() << cp->mimeData()->text();

    return a.exec();
}
