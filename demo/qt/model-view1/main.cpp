#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTableWiew tableView;
    MyModel myModel;


    return a.exec();
}
