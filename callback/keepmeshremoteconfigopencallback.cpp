#include "keepmeshremoteconfigopencallback.h"
#include <QDebug>

KeepMeshRemoteConfigOpenCallback::KeepMeshRemoteConfigOpenCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh, bool firstInvoke)
    : AsyncApiRequestTaskCallback(invoker, alerter)
{
    this->m_firstInvoke=firstInvoke;
}


void KeepMeshRemoteConfigOpenCallback::taskStarted()
{
    if(m_firstInvoke)
    {
        this->alerter()->setStatusText("正在打开远程配置功能 ...");
    }
}

void KeepMeshRemoteConfigOpenCallback::taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask)
{
    if(m_firstInvoke)
    {
        if(ok && asyncApiRequestTask->responseJson()->value("result").toString()=="success")
        {
            this->alerter()->setStatusText("打开远程配置功能成功!");
            emit firstInvokeKeepMeshRemoteConfigOpenFinished(true);
        }
        else
        {
            this->alerter()->setStatusText("打开远程配置功能出错!");
            emit firstInvokeKeepMeshRemoteConfigOpenFinished(false);
        }
    }
    asyncApiRequestTask->destroy();
}
