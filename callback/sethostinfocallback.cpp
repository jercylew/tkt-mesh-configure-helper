#include "sethostinfocallback.h"
#include "domain/tktmesh.h"

SetHostInfoCallback::SetHostInfoCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh)
    : AsyncApiRequestTaskCallback(invoker, alerter)
{
    this->m_tktMesh=tktMesh;
}

void SetHostInfoCallback::taskStarted()
{
    this->alerter()->showPopupStatusText("设置中...", true);
}

void SetHostInfoCallback::taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask)
{
    this->alerter()->hidePopupStatusText();
    if(ok && asyncApiRequestTask->responseJson()->value("result").toString()=="success")
    {
        QJsonObject data=asyncApiRequestTask->responseJson()->value("data").toObject();
        QString hostId=data.value("host_id").toString();
        if(hostId==m_tktMesh->hostID())
        {
            m_tktMesh->setHostCode(data.value("host_code").toString());

            QJsonObject hostData=data.value("host_data").toObject();
            m_tktMesh->setHostName(hostData.value("host_name").toString());
            m_tktMesh->setHostDescription(hostData.value("host_description").toString());
            m_tktMesh->setHostRegistered(hostData.value("host_registered").toBool());
            this->invoker()->reloadHostInfo();
        }
        this->alerter()->setStatusText("设置主机信息成功!", "#0000FF");
    }
    else
    {
        this->alerter()->setStatusText(QObject::tr("设置主机信息失败!"), "#FF0000");
    }

    asyncApiRequestTask->destroy();
}
