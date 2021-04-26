#include "getallcomcallback.h"
#include "abstractcallbackinvoker.h"
#include "domain/mesh.h"
#include <QJsonObject>
#include <QJsonArray>

GetAllComCallback::GetAllComCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh)
    : AsyncApiRequestTaskCallback(invoker, alerter)
{
    this->m_tktMesh=tktMesh;
}

void GetAllComCallback::taskStarted()
{
}

void GetAllComCallback::taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask)
{
    if(ok && asyncApiRequestTask->responseJson()->value("result").toString()=="success")
    {
        QJsonArray array=asyncApiRequestTask->responseJson()->value("data").toArray();
        QList<QString> comList;
        for(int i=0; i<array.size(); i++)
        {
            QJsonObject obj=array.at(i).toObject();
            QString comName=obj.value("name").toString().trimmed();
            if(comName.isEmpty())
            {
                continue;
            }
            comList.push_back(comName);
        }
        this->invoker()->loadComList(comList);
    }
    else
    {
        this->alerter()->setStatusText("获取串口列表发生错误!", "#FF0000");
    }
    asyncApiRequestTask->destroy();
}
