#ifndef SINGLEAPP_H
#define SINGLEAPP_H

#include <QCoreApplication>
#include <QLocalServer>
#include <QLocalSocket>
#include <QSharedMemory>

/**
 * @brief 此类使系统只运行一个此类的实例
 * @see QCoreApplication
 */
class SingleApp : public QCoreApplication
{
    struct InstancesInfo
    {
        bool                primary;
        quint32             secondary;
        qint64              primaryPid;
        quint16             checksum;
    };

    struct ConnectionInfo
    {
        explicit ConnectionInfo() : msgLen(0), instanceId (0), stage (0) {}
        qint64              msgLen;
        quint32             instanceId;
        quint8              stage;
    };

    Q_OBJECT

public:
    /**
     * @brief 单例相关操作
     * 共享块应该是用户范围的还是系统范围的, 以及在启动辅助实例时是否应该通知主实例
     *
     * @note 操作系统可以将共享内存块限制为同一个用户，在这种情况下，用户/系统模式将不起作用，块将是用户范围的
     * @enum
     */
    enum Mode
    {
        User                    = 1 << 0,
        System                  = 1 << 1,
        SecondaryNotification   = 1 << 2,
        ExcludeAppVersion       = 1 << 3,
        ExcludeAppPath          = 1 << 4
    };

    enum ConnectionType : quint8
    {
        InvalidConnection       = 0,
        NewInstance             = 1,
        SecondaryInstance       = 2,
        Reconnect               = 3
    };

    enum ConnectionStage : quint8
    {
        StageHeader             = 0,
        StageBody               = 1,
        StageConnected          = 2,
    };

    Q_DECLARE_FLAGS(Options, Mode)

    /**
     * @brief 初始化一个单例程序
     * @param argc 在 argv 中的命令行参数个数
     * @param argv 具体的命令行参数
     * @param appName 应用程序取名
     * @param allowSecondary 已经运行一个实例的前提下, 是否允许运行其它实例
     * @param options 在用户范围还是在系统范围应用单个应用程序块
     * @param timeout 超时时间
     *
     * @note Mode::SecondaryNotification 仅仅在允许多个实例运行的时候起作用
     * @note 超时只是对阻塞操作的最大时间的一个提示。它不能保证SingleApplicatio初始化将在给定的时间内完成
     */
    SingleApp (int &argc, char *argv[], const char *appName = "SingleApplication", bool allowSecondary = false, Options options = Mode::User, int timeout = 1000);
    ~SingleApp ();

    /**
     * @brief 此实例是否第一个实例
     * @return 返回此实例是否第一个实例
     * @retval bool
     */
    bool isPrimary ();

    /**
     * @brief 此实例是否非第一个实例
     * @return 返回此实例是否非第一个实例
     * @retval bool
     */
    bool isSecondary ();

    /**
     * @brief 第一个实例的进程id
     * @return 返回第一个实例的进程id
     * @retval qint64
     */
    qint64 primaryPid ();

    /**
     * @brief 当前实例的唯一 id
     * @return 返回当前实例的唯一编号
     * @retval quint32
     */
    quint32 instanceId ();

    /**
     * @brief 给第一个实例发送信息
     * @param message 要发送的数据
     * @param timeout 超时时间
     * @return 是否成功
     * @retval bool
     * @note 第一个实例调用将一定返回 false
     */
    bool sendMessage (QByteArray message, int timeout = 100);


private:
    void startPrimary();
    void startSecondary();
    quint16 blockChecksum();
    void initializeMemoryBlock();
    void genBlockServerName(const char *appName);
    void readInitMessageBody(QLocalSocket *socket);
    void readInitMessageHeader(QLocalSocket *socket);
    void connectToPrimary(int msecs, ConnectionType connectionType);


public Q_SLOTS:
    void slotConnectionEstablished();
    void slotDataAvailable(QLocalSocket* sock, quint32 id);
    void slotClientConnectionClosed(QLocalSocket* sock, quint32 id);

Q_SIGNALS:
    /**
     * @brief 实例启动成功
     */
    void instanceStarted ();

    /**
     * @brief 接收到信息
     * @param instanceId 实例ID
     * @param message 发送的信息
     */
    void receivedMessage (quint32 instanceId, QByteArray message);

private:
    QSharedMemory*                          mMemory;
    QLocalSocket                            *mSocket;
    QLocalServer                            *mServer;
    quint32                                 mInstanceNumber;
    QString                                 mBlockServerName;
    Options                                 mOptions;
    QMap<QLocalSocket*, ConnectionInfo>     mConnectionMap;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SingleApp::Options)

#endif // SINGLEAPP_H
