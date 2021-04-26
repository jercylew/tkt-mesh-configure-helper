#include "refreshmeshnodescallback.h"
#include "abstractcallbackinvoker.h"
#include "domain/mesh.h"
#include "model/tktmeshmodel.h"
#include <QJsonArray>
#include "domain/bluetoothnode.h"

RefreshMeshNodesCallback::RefreshMeshNodesCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMeshModel *tktMeshModel)
    : AsyncApiRequestTaskCallback(invoker, alerter)
{
    this->m_tktMeshModel=tktMeshModel;
}

void RefreshMeshNodesCallback::taskStarted()
{
    this->alerter()->showPopupStatusText("刷新中 ...", true);
}

void RefreshMeshNodesCallback::taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask)
{
    this->alerter()->hidePopupStatusText();
    if(ok && asyncApiRequestTask->responseJson()->value("result").toString()=="success")
    {
        this->alerter()->setStatusText("刷新Mesh节点成功，请点击\"获取所有节点\"来加载Mesh节点列表到显示区域!", "#0000FF");
    }
    else
    {
        this->alerter()->setStatusText("获取节点列表失败!", "#FF0000");
    }

    asyncApiRequestTask->destroy();
}
