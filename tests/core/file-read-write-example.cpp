/*************************************************************************
> FileName: file-read-write-example.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年04月20日 星期二 20时37分17秒
 ************************************************************************/
#include <cstdio>
#include <QProcess>
#include <qt/QtCore/QCoreApplication>
#include "../../core/file-read-write.h"

int main (int argc, char* argv[])
{
    QCoreApplication app (argc, argv);

    GError* error = nullptr;

    QProcess pos;
    pos.start("fallocate -l500MB /tmp/src.txt");
    pos.waitForFinished(500);

    pos.start("rm -fr /tmp/dest.txt");
    pos.waitForFinished(500);

    printf ("start file read write test ...\n");

    dingjing::FileReadAndWrite f ("file:///tmp/src.txt",
            "file:///tmp/dest.txt",
            G_FILE_COPY_OVERWRITE, NULL, NULL, NULL, &error);
    if (nullptr != error) {
        printf ("copy error: %d --- %s\n", error->code, error->message);
    }

    printf ("file read write test finished!!!\n");

    return app.exec();
}
