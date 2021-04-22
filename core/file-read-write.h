/*************************************************************************
> FileName: file-read-write.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年04月20日 星期二 15时58分22秒
 ************************************************************************/
#ifndef _FILE_READ_WRITE_H
#define _FILE_READ_WRITE_H

#include <gio/gio.h>

#include <qt/QtCore/QMutex>
#include <qt/QtCore/QObject>

namespace dingjing
{
    class FileReadAndWrite : public QObject
    {
        Q_OBJECT
    public:
        enum Status
        {
            INVALID,
            PAUSE,
            CANCEL,
            RUNNING,
            FINISHED,
            ERROR
        };
        Q_ENUM(Status)
        /**
         * @brief 将srcUri的文件内容读取并写入到destUri文件里
         * @param srcUri 复制的源文件
         * @param destUri 复制到目的地文件
         */
        explicit FileReadAndWrite (QString srcUri, QString destUri, GFileCopyFlags flags, GCancellable* cancel, GFileProgressCallback cb, gpointer pcd, GError** error, QObject* obj = nullptr);
        ~FileReadAndWrite ();

    public Q_SLOTS:
        /**
         * @brief 暂停执行
         */
        void pause ();
        /**
         * @brief 暂停后恢复
         */
        void restart ();

    private Q_SLOTS:
        /**
         * @brief 整个复制流程在这里
         */
        void run ();

    private:
        void updateProgress () const;
        void detailError (GError** error);

    private:
        QMutex                  mPause;
        GFile*                  mSrcFile = nullptr;
        GFile*                  mDestFile = nullptr;
        GFileProgressCallback   mProgress;
        GFileCopyFlags          mCopyFlags;
        GCancellable*           mCancel = nullptr;          // temp param

        GError**                mError = nullptr;           // temp param
        gpointer                mProgressData;

        goffset                 mOffset = 0;                // 记录当前进度
        goffset                 mTotalSize = 0;             // 记录当前进度
        enum Status             mStatus = INVALID;          // 记录运行状态
    };
}

#endif
