#include "setmeshruntimeparamscallback.h"
#include "abstractcallbackinvoker.h"
#include "domain/mesh.h"

SetMeshRuntimeParamsCallback::SetMeshRuntimeParamsCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh)
    : AsyncApiRequestTaskCallback(invoker, alerter)
{
    this->m_tktMesh=tktMesh;
}

void SetMeshRuntimeParamsCallback::taskStarted()
{
    this->alerter()->showPopupStatusText("设置中...", true);
}

void SetMeshRuntimeParamsCallback::taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask)
{
    this->alerter()->hidePopupStatusText();
    if(ok && asyncApiRequestTask->responseJson()->value("result").toString()=="success")
    {
        this->alerter()->setStatusText("设置Mesh信息成功!", "#0000FF");
    }
    else
    {
        this->alerter()->setStatusText("设置Mesh信息失败!", "#FF0000");
    }

    asyncApiRequestTask->destroy();
}

