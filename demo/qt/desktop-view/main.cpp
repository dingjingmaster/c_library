#include "main-widget.h"

#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;

    w.show();
    return a.exec();
}
