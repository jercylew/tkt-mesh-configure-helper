#include "getmeshlogcallback.h"
#include <QJsonArray>

GetMeshLogCallback::GetMeshLogCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh, bool alert)
    : AsyncApiRequestTaskCallback(invoker, alerter)
{
    this->m_tktMesh=tktMesh;
    this->m_alert=alert;
}

void GetMeshLogCallback::taskStarted()
{
    if(m_alert)
    {
        this->alerter()->showPopupStatusText("获取中 ...", true);
    }
    else
    {
        this->alerter()->setStatusText("获取中...", "#000000");
    }
}

void GetMeshLogCallback::taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask)
{
    if(m_alert)
    {
        this->alerter()->hidePopupStatusText();
    }
    if(ok && asyncApiRequestTask->responseJson()->value("result").toString()=="success")
    {
        QJsonArray array=asyncApiRequestTask->responseJson()->value("data").toArray();
        QList<QString> logList;
        for(int i=0; i<array.size(); i++)
        {
            logList.push_back(array.at(i).toString());
        }
        this->invoker()->loadMeshLog(&logList);
        this->alerter()->setStatusText("获取MESH日志信息成功!", "#0000FF");
    }
    else
    {
        this->alerter()->setStatusText("获取MESH日志信息失败!", "#FF0000");
    }

    asyncApiRequestTask->destroy();
}


