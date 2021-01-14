#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMessageBox w;
    QMessageBox::information(nullptr, "title", "text");
    w.information(nullptr, "title", "text");
    w.show();
    return a.exec();
}
