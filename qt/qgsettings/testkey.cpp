#include "testkey.h"
#include <QDebug>

TestKey::TestKey(QObject *parent) : QObject(parent)
{

}

void TestKey::testKey(QString key)
{
    qDebug() << "in !!!!!!!";
    qDebug() << key;
}
