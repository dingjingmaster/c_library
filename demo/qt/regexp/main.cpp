#include <QCoreApplication>
#include <QRegExp>
#include <QDebug>

int main(int argc, char *argv[])
{
    QString str1 = "asdsad(1).txt";
    QString str2 = "bbbb(2)kkk(1)";
    QString str3 = "bbbb(2)kkk(1).txt";
    QString str4 = "bbbb(2)kkk";

    QRegExp regx("\\(\\d+\\)(\\.[0-9a-zA-Z]+|)$");

    qDebug() << regx.indexIn(str1) << "   " << regx.cap(0);
    qDebug() << regx.indexIn(str2) << "   " << regx.cap(0);
    qDebug() << regx.indexIn(str3) << "   " << regx.cap(0);
    qDebug() << regx.indexIn(str4) << "   " << regx.cap(0);


    Q_UNUSED(argc)
    Q_UNUSED(argv)
    return 0;
}
