#include <QCoreApplication>
#include <QRegExp>
#include <QDebug>

int main(int argc, char *argv[])
{
    QString str1 = "asdsad(1).txt";
    QString str2 = "bbbb(2)kkk(1)";
    QString str3 = "bbbb(2)kkk(1).txt";

    QRegExp regx("\\(\\d+\\)(\\.[a-zA-Z]+|)$");

    qDebug() << regx.indexIn(str1);
    qDebug() << regx.indexIn(str2);
    qDebug() << regx.indexIn(str3);

    Q_UNUSED(argc)
    Q_UNUSED(argv)
    return 0;
}
