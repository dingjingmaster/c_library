/*************************************************************************
> FileName: file-read-write.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年04月20日 星期二 15时58分38秒
 ************************************************************************/
#include "file-read-write.h"
#include <cstdlib>
#include <cstring>

#define BUF_SIZE        1024000

using namespace dingjing;

FileReadAndWrite::FileReadAndWrite (QString srcUri, QString destUri, GFileCopyFlags flags, GCancellable* cancel, GFileProgressCallback cb, gpointer pcd, GError** error, QObject* obj) : QObject (obj)
{
    // 默认两个都是文件
    mSrcFile = g_file_new_for_uri(srcUri.toUtf8());
    mDestFile = g_file_new_for_uri(destUri.toUtf8());

    mCopyFlags = flags;

    mCancel = cancel;
    mProgress = cb;
    mProgressData = pcd;
    mError = error;

    run();
}

FileReadAndWrite::~FileReadAndWrite()
{
    if (nullptr != mReadIO)         g_object_unref(mReadIO);
    if (nullptr != mWriteIO)        g_object_unref(mWriteIO);
    if (nullptr != mSrcFile)        g_object_unref(mSrcFile);
    if (nullptr != mDestFile)       g_object_unref(mDestFile);
    if (nullptr != mReadCancel)     g_object_unref(mReadCancel);
    if (nullptr != mWriteCancel)    g_object_unref(mWriteCancel);
}


void FileReadAndWrite::pause ()
{
    if (RUNNING == mStatus) {
        mStatus = PAUSE;
    }
}

void FileReadAndWrite::restart ()
{
    if (PAUSE == mStatus) {
        mStatus = RUNNING;
        mPause.unlock();
    }
}

void FileReadAndWrite::run ()
{
    GError*                 error = NULL;
    gssize                  readSize = 0;
    gssize                  writeSize = 0;
    char                    buf[BUF_SIZE] = {0};
    GError*                 readError = nullptr;
    GError*                 writeError = nullptr;
    if (nullptr != mSrcFile) {
        mReadIO = g_file_read(mSrcFile, mReadCancel, &readError);
        // 处理错误并退出
        if (nullptr != readError) {
            printf ("read io %s\n", readError->message);
        }
    }

    if (nullptr != mDestFile) {
        mWriteIO = g_file_create(mDestFile, G_FILE_CREATE_REPLACE_DESTINATION, mWriteCancel, &writeError);
        // 处理错误并退出 配置标记位
        if (nullptr != writeError) {
            printf ("write io %s\n", writeError->message);
        }
    }

    if (!mReadIO || !mWriteIO) {
        // 错误处理
        printf ("io error!\n");
        return;
    }

    while (true) {
        if (RUNNING == mStatus) {
            memset(buf, 0, sizeof(buf));
            // 检查读取是否结束
            readSize = g_input_stream_read(G_INPUT_STREAM(mReadIO), buf, sizeof(buf) - 1, NULL, &error);
            if (0 == readSize && NULL == error) {
                // 读取结束
                mStatus = FINISHED;
                continue;
            } else if (readSize < 0 || NULL != error) {
                // 读取出错,返回错误信息
            }

            // 写
            writeSize = g_output_stream_write(G_OUTPUT_STREAM(mWriteIO), buf, readSize, NULL, &error);
            // 判断是否都写入了
            mOffset += writeSize;

            // 更新进度条
            if (nullptr != mProgress) {
                mProgress(mOffset, mTotalSize, mProgressData);
            }

        } else if (CANCEL == mStatus) {
            // fixme://补齐错误信息
            g_set_error(mError, 0, G_IO_ERROR_CANCELLED, "");
            break;
        } else if (PAUSE == mStatus) {
            mPause.lock();
            continue;
        } else if (FINISHED == mStatus) {
            break;
        } else {
            mStatus = RUNNING;
        }
    }

    // async 操作
    g_output_stream_flush(G_OUTPUT_STREAM(mWriteIO), NULL, &error);
    if (NULL != error) {
        // 出错....
        printf("%s\n", error->message);
    }


}
