#include "style-window.h"

#include <QStyleFactory>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("simplestyle"));

    StyleWindow w;
    w.resize(300, 100);
    w.show();

    return a.exec();
}
