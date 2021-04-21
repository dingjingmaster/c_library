/*************************************************************************
> FileName: file-read-write-example.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年04月20日 星期二 20时37分17秒
 ************************************************************************/
#include <cstdio>
#include <gio/gio.h>
#include <qt/QtCore/QCoreApplication>

#include "../../core/file-read-write.h"

int main (int argc, char* argv[])
{
    QCoreApplication app (argc, argv);

    printf ("ok!!!\n");

    dingjing::FileReadAndWrite f ("file:///data/code/c_library/tests/core/src.txt", "file:///data/code/c_library/tests/core/dest.txt", G_FILE_COPY_OVERWRITE, NULL, NULL, NULL, NULL);




    return app.exec();
}
