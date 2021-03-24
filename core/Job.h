//
// Created by dingjing on 2021/3/24.
//

#ifndef LIBCORE_DINGJING_JOB_H
#define LIBCORE_DINGJING_JOB_H
#include <memory>
#include <QThread>
#include <QObject>
#include <QtGlobal>
#include <QRunnable>
#include <gio/gio.h>

#include "smart-ptr.hpp"

namespace dingjing
{

class EXPORT_IMPORT_LIBRARY Job : public QObject, public QRunnable
{
    Q_OBJECT
public:
    enum class ErrorAction
    {
        CONTINUE,
        RETRY,
        ABORT
    };

    enum class ErrorSeverity
    {
        UNKNOWN,
        WARNING,
        MILD,
        MODERATE,
        SEVERE,
        CRITICAL
    };

    explicit Job();
    ~Job() override;

    bool isCancelled() const;

    void runAsync(QThread::Priority priority = QThread::InheritPriority);

    bool pause();

    void resume();

    const GCancellablePtr& cancellable() const;

Q_SIGNALS:
    void cancelled();

    void finished();

    // this signal should be connected with Qt::BlockingQueuedConnection
    void error(const GErrorPtr& err, ErrorSeverity severity, ErrorAction& response);

    public Q_SLOTS:

    void cancel();

    void run() override;

protected:
    ErrorAction emitError(const GErrorPtr& err, ErrorSeverity severity = ErrorSeverity::MODERATE);

    // all derived job subclasses should do their work in this method.
    virtual void exec() = 0;

private:
    static void _onCancellableCancelled(GCancellable* cancellable, Job* _this);

    void onCancellableCancelled(GCancellable* /*cancellable*/);

private:
    bool paused_;
    GCancellablePtr cancellable_;
    gulong cancellableHandler_;
};

}
#endif //LIBCORE_DINGJING_JOB_H
