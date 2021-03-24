//
// Created by dingjing on 2021/3/24.
//

#include "Job.h"
#include "job_p.h"

namespace dingjing {

    Job::Job() : paused_{false}, cancellable_{g_cancellable_new(), false}, cancellableHandler_ {
        g_signal_connect(cancellable_.get(), "cancelled", G_CALLBACK(_onCancellableCancelled), this)} {}


    Job::~Job()
    {
        if(cancellable_) {
            g_cancellable_disconnect(cancellable_.get(), cancellableHandler_);
        }
    }

    void Job::runAsync(QThread::Priority priority)
    {
        auto thread = new JobThread(this);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        if(autoDelete()) {
            connect(this, &Job::finished, this, &Job::deleteLater);
        }
        thread->start(priority);
    }

    void Job::cancel()
    {
        g_cancellable_cancel(cancellable_.get());
    }

    bool Job::isCancelled() const
    {
        return g_cancellable_is_cancelled(cancellable_.get());
    }

    const GCancellablePtr& Job::cancellable() const
    {
        return cancellable_;
    }

    static void Job::_onCancellableCancelled(GCancellable* cancellable, Job* _this)
    {
        _this->onCancellableCancelled(cancellable);
    }

    void Job::onCancellableCancelled(GCancellable* /*cancellable*/)
    {
        Q_EMIT cancelled();
    }

    void Job::run()
    {
        exec();
        Q_EMIT finished();
    }

    Job::ErrorAction Job::emitError(const GErrorPtr &err, Job::ErrorSeverity severity)
    {
        ErrorAction response = ErrorAction::CONTINUE;
        // if the error is already handled, don't emit it.
        if(err.domain() == G_IO_ERROR && err.code() == G_IO_ERROR_FAILED_HANDLED) {
            return response;
        }
        Q_EMIT error(err, severity, response);

        if(severity == ErrorSeverity::CRITICAL || response == ErrorAction::ABORT) {
            cancel();
        } else if (response == ErrorAction::RETRY) {
            /* If the job is already cancelled, retry is not allowed. */
            if(isCancelled() || (err.domain() == G_IO_ERROR && err.code() == G_IO_ERROR_CANCELLED)) {
                response = ErrorAction::CONTINUE;
            }
        }

        return response;
    }
}
