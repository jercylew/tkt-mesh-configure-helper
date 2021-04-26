#include "gethostinfocallback.h"
#include "domain/tktmesh.h"

GetHostInfoCallback::GetHostInfoCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh)
    : AsyncApiRequestTaskCallback(invoker, alerter)
{
    this->m_tktMesh=tktMesh;
}

void GetHostInfoCallback::taskStarted()
{
    this->alerter()->showPopupStatusText("正在获取 ...", true);
}

void GetHostInfoCallback::taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask)
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
            m_tktMesh->setPlatformAddress(hostData.value("platform_address").toString());
            m_tktMesh->setPlatformPort(hostData.value("platform_port").toInt());
            m_tktMesh->setCloudConfigAddress(hostData.value("cloud_config_address").toString());
            m_tktMesh->setCloudConfigPort(hostData.value("cloud_config_port").toInt());
            this->invoker()->reloadHostInfo();
        }
        this->alerter()->setStatusText("获取TKTMesh主机信息成功!", "#0000FF");
    }
    else
    {
        this->alerter()->setStatusText("获取TKTMesh主机信息失败!", "#FF0000");
    }

    asyncApiRequestTask->destroy();
}
