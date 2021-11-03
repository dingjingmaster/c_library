#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

    QMessageBox b;

    b.setIconPixmap (QIcon::fromTheme ("audio-x-mpeg").pixmap (64, 64));

    b.show ();

    return a.exec();
}
