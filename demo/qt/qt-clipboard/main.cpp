#include "clipboard-utils.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ClipboardUtils cu;

    return a.exec();
}
