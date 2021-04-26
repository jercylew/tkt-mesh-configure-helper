#include "setmeshbasicinfocallback.h"
#include "abstractcallbackinvoker.h"
#include "domain/mesh.h"

SetMeshBasicInfoCallback::SetMeshBasicInfoCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh)
    : AsyncApiRequestTaskCallback(invoker, alerter)
{
    this->m_tktMesh=tktMesh;
}

void SetMeshBasicInfoCallback::taskStarted()
{
    this->alerter()->showPopupStatusText("设置中...", true);
}

void SetMeshBasicInfoCallback::taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask)
{
    this->alerter()->hidePopupStatusText();
    if(ok && asyncApiRequestTask->responseJson()->value("result").toString()=="success")
    {
        this->alerter()->setStatusText("设置MESH运行时参数成功!", "#0000FF");
    }
    else
    {
        this->alerter()->setStatusText("设置MESH运行时参数失败!", "#FF0000");
    }

    asyncApiRequestTask->destroy();
}
