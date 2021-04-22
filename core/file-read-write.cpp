/*************************************************************************
> FileName: file-read-write.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年04月20日 星期二 15时58分38秒
 ************************************************************************/
#include "file-read-write.h"
#include <cstring>
#include <QString>

#define BUF_SIZE        1024000

using namespace dingjing;

FileReadAndWrite::FileReadAndWrite (QString srcUri, QString destUri, GFileCopyFlags flags, GCancellable* cancel, GFileProgressCallback cb, gpointer pcd, GError** error, QObject* obj) : QObject (obj)
{
    // 此处判断两个文件情况，并做一些处理

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
    if (nullptr != mSrcFile)        g_object_unref(mSrcFile);
    if (nullptr != mDestFile)       g_object_unref(mDestFile);
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

void FileReadAndWrite::detailError (GError** error)
{
    if (nullptr == error || nullptr == *error || nullptr == mError) {
        return;
    }

    g_set_error(mError, (*error)->domain, (*error)->code, (*error)->message);
    g_error_free(*error);

    *error = nullptr;
}


void FileReadAndWrite::updateProgress () const
{
    if (nullptr == mProgress) {
        return;
    }

    mProgress(mOffset, mTotalSize, mProgressData);
}

void FileReadAndWrite::run ()
{
    GError*                 error = nullptr;
    gssize                  readSize = 0;
    gssize                  writeSize = 0;
    char                    buf[BUF_SIZE] = {0};
    GFileInputStream*       readIO = nullptr;
    GFileOutputStream*      writeIO = nullptr;

    // it's impossible
    if (nullptr == mSrcFile || nullptr == mDestFile) {
        *mError = g_error_new (0, G_IO_ERROR_INVALID_ARGUMENT,tr("Error in source or destination file path!").toUtf8());
        goto out;
    }

    readIO = g_file_read(mSrcFile, nullptr, &error);
    if (nullptr != error) {
        detailError(&error);
        goto out;
    }

    // 针对目标文件存在与否做预处理 --

    writeIO = g_file_create(mDestFile, G_FILE_CREATE_REPLACE_DESTINATION, nullptr, &error);
    if (nullptr != error) {
        detailError(&error);
        goto out;
    }

    if (!readIO || !writeIO) {
        *mError = g_error_new (0, G_IO_ERROR_FAILED,tr("Error opening source or destination file!").toUtf8());
        goto out;
    }

    while (true) {
        if (RUNNING == mStatus) {
            if (nullptr != mCancel && g_cancellable_is_cancelled(mCancel)) {
                mStatus = CANCEL;
                continue;
            }

            memset(buf, 0, sizeof(buf));
            readSize = g_input_stream_read(G_INPUT_STREAM(readIO), buf, sizeof(buf) - 1, nullptr, &error);
            if (0 == readSize && nullptr == error) {
                mStatus = FINISHED;
                continue;
            } else if (nullptr != error) {
                detailError(&error);
                mStatus = ERROR;
                continue;
            }

            writeSize = g_output_stream_write(G_OUTPUT_STREAM(writeIO), buf, readSize, nullptr, &error);
            if (nullptr != error) {
                detailError(&error);
                mStatus = ERROR;
                continue;
            }

            if (readSize != writeSize) {
                // it's impossible
                *mError = g_error_new (0, G_IO_ERROR_FAILED,tr("Reading and Writing files are inconsistent!").toUtf8());
                mStatus = ERROR;
                continue;
            }

            mOffset += writeSize;
            updateProgress ();

        } else if (CANCEL == mStatus) {
            g_set_error(mError, 0, G_IO_ERROR_CANCELLED, tr("operation cancel").toUtf8());
            g_file_delete (mDestFile, nullptr, nullptr);
            break;
        }else if (ERROR == mStatus) {
            g_file_delete (mDestFile, nullptr, nullptr);
            break;
        } else if (PAUSE == mStatus) {
            mPause.lock();
            continue;
        } else if (FINISHED == mStatus) {
            g_output_stream_flush(G_OUTPUT_STREAM(writeIO), nullptr, &error);
            if (nullptr != error) {
                detailError(&error);
                mStatus = ERROR;
                continue;
            }
            break;
        } else {
            mStatus = RUNNING;
        }
    }


out:
    if (nullptr != readIO) {
        g_input_stream_close (G_INPUT_STREAM(readIO), nullptr, nullptr);
        g_object_unref(readIO);
    }

    if (nullptr != writeIO) {
        g_output_stream_close (G_OUTPUT_STREAM(writeIO), nullptr, nullptr);
        g_object_unref(writeIO);
    }

    if (nullptr != error) {
        g_error_free(error);
    }
}
