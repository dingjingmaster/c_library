#include <QApplication>
#include <QMainWindow>

#include <pathbar-model-view/pathbar.h>

using namespace dingjing;

int main (int argc, char *argv[])
{
    QApplication app (argc, argv);

    QMainWindow w;

    PathBar pb(&w);

    w.show();

    return app.exec();
}
