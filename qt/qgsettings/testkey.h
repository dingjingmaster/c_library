#ifndef TESTKEY_H
#define TESTKEY_H

#include <QObject>

class TestKey : public QObject
{
    Q_OBJECT
public:
    explicit TestKey(QObject *parent = nullptr);

public Q_SLOTS:
    void testKey(QString);

};

#endif // TESTKEY_H
