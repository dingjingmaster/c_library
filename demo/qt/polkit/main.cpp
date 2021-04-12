#include <QCoreApplication>
#include <polkit-qt5-1/polkitqt1-authority.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return a.exec();
}
