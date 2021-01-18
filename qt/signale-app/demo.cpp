#include "single-app.h"

int main (int argc, char* argv[])
{
    SingleApp app (argc, argv, "demo", false);

    if (app.isSecondary()) {
        qDebug() << "secondary demo";
    }

    return app.exec();
}
