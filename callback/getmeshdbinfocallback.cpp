#include "getmeshdbinfocallback.h"

#include "getmeshdbdatacallback.h"
#include "model/meshmodel.h"
#include "utils/remoteconfigmanager.h"
#include "utils/remoteconfigsessionholder.h"
#include <QDebug>
#include "model/preparemeshmodel.h"

GetMeshDBInfoCallback::GetMeshDBInfoCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, PrepareMeshModel *prepareMeshModel)
   : AsyncApiRequestTaskCallback(invoker, alerter)
{
    this->m_prepareMeshModel=prepareMeshModel;
}

void GetMeshDBInfoCallback::taskStarted()
{

}

void GetMeshDBInfoCallback::taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask)
{
    if(ok && asyncApiRequestTask->responseJson()->value("result").toString()=="success")
    {
        QJsonObject dataJson=asyncApiRequestTask->responseJson()->value("data").toObject();
        quint64 total=dataJson.value("total").toString().toULongLong();
        QString md5=dataJson.value("md5").toString();
        qDebug()<<"=============total="<<total<<", md5="<<md5;
        this->md5=md5;
        this->total=total;
        emit getMeshDBInfoFinished(true);
    }
    else
    {
        emit getMeshDBInfoFinished(false);
    }
    asyncApiRequestTask->destroy();
}

QString GetMeshDBInfoCallback::getMd5()
{
    return md5;
}

void GetMeshDBInfoCallback::setMd5(const QString &value)
{
    md5 = value;
}

quint64 GetMeshDBInfoCallback::getTotal()
{
    return total;
}

void GetMeshDBInfoCallback::setTotal(const quint64 &value)
{
    total = value;
}
