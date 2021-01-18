#include "single-app.h"
#include <pwd.h>
#include <unistd.h>

#include <QTime>
#include <QThread>
#include <QDataStream>
#include <QCryptographicHash>

#define SHARED_MEMORY_SIZE 4096

SingleApp::SingleApp(int &argc, char *argv[], const char *appName, bool allowSecondary, Options options, int timeout)
        : QCoreApplication (argc, argv)
{
    bool            ret = false;
    unsigned long   blockSize = 0;

    mServer = nullptr;
    mSocket = nullptr;
    mMemory = nullptr;
    mInstanceNumber = -1;

    mOptions = options;

    genBlockServerName(appName);
    qDebug() << "memory name:" << mBlockServerName;

#ifdef Q_OS_UNIX
    // 防止异常退出后, 没有销毁共享内存
    mMemory = new QSharedMemory(mBlockServerName);
    mMemory->attach();
    delete mMemory;
#endif

    // 初始化共享内存
    mMemory = new QSharedMemory(mBlockServerName);
    while (++blockSize) {
        if (blockSize * SHARED_MEMORY_SIZE >= sizeof (InstancesInfo)) {
            ret = mMemory->create(sizeof (InstancesInfo));
            qDebug() << "create shared memary success!";
            break;
        }
    }

    if(ret) {
        mMemory->lock();
        initializeMemoryBlock();
        mMemory->unlock();
    } else {
        if (!mMemory->attach()) {
            qCritical() << "SingleApplication: Unable to attach to shared memory block. error:" << mMemory->errorString();
            ::exit( EXIT_FAILURE );
        }
    }

    mMemory->lock();
    InstancesInfo* inst = static_cast<InstancesInfo*>(mMemory->data());
    QTime time;
    time.start();

    while(true) {
        if(blockChecksum() == inst->checksum) break;
        if(time.elapsed() > 5000) {
            qWarning() << "SingleApplication: Shared memory block has been in an inconsistent state from more than 5s. Assuming primary instance failure.";
            initializeMemoryBlock();
        }

        qsrand(QDateTime::currentMSecsSinceEpoch() % std::numeric_limits<uint>::max());
        QThread::sleep(8 + static_cast<unsigned long>(static_cast<float>(qrand()) / RAND_MAX * 10));
    }

    if (false == inst->primary) {
        qDebug() << "start primary";
        startPrimary();
        mMemory->unlock();
        return;
    }

    if(allowSecondary) {
        qDebug() << "start other app";
        inst->secondary += 1;
        inst->checksum = blockChecksum();
        mInstanceNumber = inst->secondary;
        startSecondary();
        if (mOptions & Mode::SecondaryNotification) {
            connectToPrimary (timeout, SecondaryInstance);
        }
        mMemory->unlock();
        return;
    }

    mMemory->unlock();

    connectToPrimary (timeout, NewInstance);

    qDebug() << "init application fail!";
    ::exit(EXIT_FAILURE);
}

SingleApp::~SingleApp()
{
    if (nullptr != mSocket) {
        mSocket->close();
        delete mSocket;
    }

    mMemory->lock();
    InstancesInfo* inst = static_cast<InstancesInfo*>(mMemory->data());
    if(mServer != nullptr) {
        mServer->close();
        delete mServer;
        inst->primary = false;
        inst->primaryPid = -1;
        inst->checksum = blockChecksum();
    }
    mMemory->unlock();

    delete mMemory;
}

bool SingleApp::isPrimary()
{
    return nullptr != mServer;
}

bool SingleApp::isSecondary()
{
    return mServer == nullptr;
}

qint64 SingleApp::primaryPid()
{
    qint64 pid;

    mMemory->lock();
    InstancesInfo* inst = static_cast<InstancesInfo*>(mMemory->data());
    pid = inst->primaryPid;
    mMemory->unlock();

    return pid;
}

quint32 SingleApp::instanceId()
{
    return mInstanceNumber;
}

bool SingleApp::sendMessage(QByteArray message, int timeout)
{
    if(isPrimary()) {
        qDebug() << "is primary, cannot send message!";
        return false;
    }

    connectToPrimary(timeout,  Reconnect);

    mSocket->write(message);
    bool dataWritten = mSocket->waitForBytesWritten(timeout);
    mSocket->flush();
    return dataWritten;
}

void SingleApp::genBlockServerName(const char *appName)
{
    QCryptographicHash appData(QCryptographicHash::Sha256);
    appData.addData (appName, 18);
    appData.addData (SingleApp::QCoreApplication::applicationName().toUtf8());
    appData.addData (SingleApp::QCoreApplication::organizationName().toUtf8());
    appData.addData (SingleApp::QCoreApplication::organizationDomain().toUtf8());

    if(!(mOptions & SingleApp::Mode::ExcludeAppVersion)) {
        appData.addData( SingleApp::QCoreApplication::applicationVersion().toUtf8());
    }

    if(!(mOptions & SingleApp::Mode::ExcludeAppPath)) {
        appData.addData(SingleApp::QCoreApplication::applicationFilePath().toUtf8());
    }

    if(mOptions & SingleApp::Mode::User) {
        QByteArray username;
        uid_t uid = geteuid();
        struct passwd *pw = getpwuid(uid);
        if(pw) {
            username = pw->pw_name;
        }
        if(username.isEmpty()) {
            username = qgetenv("USER");
        }
        appData.addData(username);
    }

    mBlockServerName = appData.result().toBase64().replace("/", "_");
}

void SingleApp::initializeMemoryBlock()
{
    InstancesInfo* inst = static_cast<InstancesInfo*>(mMemory->data());
    inst->primary = false;
    inst->secondary = 0;
    inst->primaryPid = -1;
    inst->checksum = blockChecksum();
}

void SingleApp::startPrimary()
{
    QLocalServer::removeServer(mBlockServerName);
    mServer = new QLocalServer();

    if(mOptions & SingleApp::Mode::User) {
        mServer->setSocketOptions(QLocalServer::UserAccessOption);
    } else {
        mServer->setSocketOptions(QLocalServer::WorldAccessOption);
    }

    mServer->listen(mBlockServerName);
    QObject::connect(mServer, &QLocalServer::newConnection, this, &SingleApp::slotConnectionEstablished);

    InstancesInfo* inst = static_cast <InstancesInfo*>(mMemory->data());

    inst->primary = true;
    inst->primaryPid = applicationPid();
    inst->checksum = blockChecksum();

    mInstanceNumber = 0;
}

void SingleApp::startSecondary()
{

}

void SingleApp::connectToPrimary(int msecs, SingleApp::ConnectionType connectionType)
{
    if(mSocket == nullptr) {
        mSocket = new QLocalSocket();
    }

    if(mSocket->state() == QLocalSocket::ConnectedState) {
        return;
    }

    if(mSocket->state() == QLocalSocket::UnconnectedState || mSocket->state() == QLocalSocket::ClosingState) {
        mSocket->connectToServer(mBlockServerName);
    }

    if(mSocket->state() == QLocalSocket::ConnectingState) {
        mSocket->waitForConnected(msecs);
    }

    if(mSocket->state() == QLocalSocket::ConnectedState) {
        QByteArray initMsg;
        QDataStream writeStream(&initMsg, QIODevice::WriteOnly);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
        writeStream.setVersion(QDataStream::Qt_5_6);
#endif

        writeStream << mBlockServerName.toLatin1();
        writeStream << static_cast<quint8>(connectionType);
        writeStream << mInstanceNumber;
        quint16 checksum = qChecksum(initMsg.constData(), static_cast<quint32>(initMsg.length()));
        writeStream << checksum;

        QByteArray header;
        QDataStream headerStream(&header, QIODevice::WriteOnly);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
        headerStream.setVersion(QDataStream::Qt_5_6);
#endif
        headerStream << static_cast <quint64>(initMsg.length());

        mSocket->write(header);
        mSocket->write(initMsg);
        mSocket->flush();
        mSocket->waitForBytesWritten(msecs);
    }
}

quint16 SingleApp::blockChecksum()
{
    return qChecksum(static_cast <const char *>(mMemory->data()), offsetof(InstancesInfo, checksum));
}

void SingleApp::readInitMessageHeader(QLocalSocket *sock)
{
    if (!mConnectionMap.contains(sock)) {
        return;
    }

    if(sock->bytesAvailable() < (qint64)sizeof(quint64)) {
        return;
    }

    QDataStream headerStream(sock);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    headerStream.setVersion(QDataStream::Qt_5_6);
#endif

    quint64 msgLen = 0;
    headerStream >> msgLen;
    ConnectionInfo &info = mConnectionMap[sock];
    info.stage = StageBody;
    info.msgLen = msgLen;

    if (sock->bytesAvailable() >= (qint64) msgLen) {
        readInitMessageBody(sock);
    }
}

void SingleApp::readInitMessageBody(QLocalSocket *sock)
{
    if (!mConnectionMap.contains(sock)) {
        return;
    }

    ConnectionInfo &info = mConnectionMap[sock];
    if(sock->bytesAvailable() < (qint64)info.msgLen) {
        return;
    }

    // Read the message body
    QByteArray msgBytes = sock->read(info.msgLen);
    QDataStream readStream(msgBytes);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    readStream.setVersion(QDataStream::Qt_5_6);
#endif

    QByteArray latin1Name;
    readStream >> latin1Name;

    ConnectionType connectionType = InvalidConnection;
    quint8 connTypeVal = InvalidConnection;
    readStream >> connTypeVal;
    connectionType = static_cast <ConnectionType>(connTypeVal);

    quint32 instanceId = 0;
    readStream >> instanceId;

    quint16 msgChecksum = 0;
    readStream >> msgChecksum;

    const quint16 actualChecksum = qChecksum(msgBytes.constData(), static_cast<quint32>(msgBytes.length() - sizeof(quint16)));

    bool isValid = readStream.status() == QDataStream::Ok && QLatin1String(latin1Name) == mBlockServerName && msgChecksum == actualChecksum;
    if(!isValid) {
        sock->close();
        return;
    }

    info.instanceId = instanceId;
    info.stage = StageConnected;

    if(connectionType == NewInstance || ( connectionType == SecondaryInstance && mOptions & SingleApp::Mode::SecondaryNotification)) {
        Q_EMIT instanceStarted();
    }

    if (sock->bytesAvailable() > 0) {
        Q_EMIT this->slotDataAvailable(sock, instanceId);
    }
}

void SingleApp::slotConnectionEstablished()
{
    QLocalSocket *nextConnSocket = mServer->nextPendingConnection();
    mConnectionMap.insert(nextConnSocket, ConnectionInfo());

    QObject::connect(nextConnSocket, &QLocalSocket::aboutToClose,
        [nextConnSocket, this]() {
            auto &info = mConnectionMap[nextConnSocket];
            Q_EMIT this->slotClientConnectionClosed(nextConnSocket, info.instanceId);
        }
    );

    QObject::connect(nextConnSocket, &QLocalSocket::disconnected,
        [nextConnSocket, this]() {
            mConnectionMap.remove(nextConnSocket);
            nextConnSocket->deleteLater();
        }
    );

    QObject::connect(nextConnSocket, &QLocalSocket::readyRead,
        [nextConnSocket, this]() {
            auto &info = mConnectionMap[nextConnSocket];
            switch(info.stage) {
            case StageHeader:
                readInitMessageHeader(nextConnSocket);
                break;
            case StageBody:
                readInitMessageBody(nextConnSocket);
                break;
            case StageConnected:
                Q_EMIT this->slotDataAvailable(nextConnSocket, info.instanceId);
                break;
            default:
                break;
            };
        }
    );
}

void SingleApp::slotDataAvailable(QLocalSocket *dataSocket, quint32 instanceId)
{
    Q_EMIT receivedMessage(instanceId, dataSocket->readAll());
}

void SingleApp::slotClientConnectionClosed(QLocalSocket *closedSocket, quint32 instanceId)
{
    if (closedSocket->bytesAvailable() > 0) {
        Q_EMIT slotDataAvailable(closedSocket, instanceId);
    }
}
