#include "clipboard-utils.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ClipboardUtils cu;
    cu.show ();

    return a.exec();
}
