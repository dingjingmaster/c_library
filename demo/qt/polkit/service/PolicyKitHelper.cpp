//
// Created by linux on 5/23/17.
//

#include "PolicyKitHelper.h"
bool PolicyKitHelper::checkAuthorization(const QString& actionId, qint64 applicationPid)
{
    Authority::Result result;
    // 第一个参数是需要验证的action，和规则文件写的保持一致
    result = Authority::instance()->checkAuthorizationSync(actionId, UnixProcessSubject(applicationPid),
                                                           Authority::AllowUserInteraction);
    if (result == Authority::Yes) {
        return true;
    }else {
        return false;
    }
}

PolicyKitHelper::PolicyKitHelper()
{

}

PolicyKitHelper::~PolicyKitHelper()
{

}