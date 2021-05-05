//
// Created by dingjing on 2021/5/5.
//

#include <QDebug>
#include <iostream>
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QCoreApplication>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server ()
    {
        mTcp = new QTcpServer(this);
        connect (mTcp, &QTcpServer::newConnection, this, &Server::newConn);
        if (!mTcp->listen(QHostAddress("127.0.0.1"), 12000)){
            std::cout << "not listening!" << std::endl \
            << "ip: " << mTcp->serverAddress().toString().toStdString()\
            << " port:" << mTcp->serverPort() << std::endl;
            exit(0);
        }
    }

public Q_SLOTS:
    void newConn ()
    {
        QTcpSocket* conn = mTcp->nextPendingConnection();

        // 保存此连接
        // 处理读写信号

        std::cout  << "ip: " << conn->localAddress().toString().toStdString()\
            << "port: " << conn->localPort() << std::endl;
        std::cout << "read ==============================" << std::endl;
        conn->waitForReadyRead();
        std::cout << conn->readAll().toStdString() << std::endl;
        std::cout << "===================================" << std::endl << std::endl;

        conn->waitForBytesWritten();
        conn->write(
                "HTTP/1.1 200 OK\n\r"
                "Access-Control-Allow-Origin: *\n\r"
                "Connection: Keep-Alive\n\r"
                "Content-Type: text/html; charset=utf-8\n\r"
                "\n\r\n\r"
                "<html><head><meta charset=\"UTF-8\"/></head>"
                "<body>"
                "aaaaaaaaaaaaaa"
                "</body>"
                "</html>"
                );
        conn->flush();
        conn->close();
    };

private:
    QTcpServer* mTcp;
};


int main (int argc, char* argv[])
{
    QCoreApplication app (argc, argv);
    Server s;


    return QCoreApplication::exec();
}

#include "tcp-service.moc"