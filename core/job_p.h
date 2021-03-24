//
// Created by dingjing on 2021/3/24.
//

#ifndef LIBCORE_DINGJING_JOB_P_H
#define LIBCORE_DINGJING_JOB_P_H

namespace dingjing
{

class JobThread: public QThread
{
    Q_OBJECT
public:
    JobThread(Job* job): job_{job} {}

protected:

    void run() override
    {
        job_->run();
    }

    Job* job_;
};

}

#endif //LIBCORE_DINGJING_JOB_P_H
