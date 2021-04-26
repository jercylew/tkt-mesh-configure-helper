#include "setmeshdbdatacallback.h"
#include "model/preparemeshmodel.h"
#include "utils/remoteconfigmanager.h"
#include "utils/remoteconfigsessionholder.h"
#include <QDebug>

SetMeshDBDataCallback::SetMeshDBDataCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, PrepareMeshModel *prepareMeshModel)
   : AsyncApiRequestTaskCallback(invoker, alerter)
{
    this->m_prepareMeshModel=prepareMeshModel;
}

void SetMeshDBDataCallback::taskStarted()
{
}

void SetMeshDBDataCallback::taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask)
{
    if(ok && asyncApiRequestTask->responseJson()->value("result").toString()=="success")
    {
        QJsonObject dataJson=asyncApiRequestTask->responseJson()->value("data").toObject();

        this->total=dataJson.value("total").toString().toULongLong();
        this->write=dataJson.value("write").toString().toULongLong();
        this->md5Ok=(dataJson.value("ok").toString()=="true");

        emit setMeshDBDataFinished(true);
    }
    else
    {
        emit setMeshDBDataFinished(false);
    }
    asyncApiRequestTask->destroy();
}

quint64 SetMeshDBDataCallback::getTotal()
{
    return total;
}

void SetMeshDBDataCallback::setTotal(const quint64 &value)
{
    total = value;
}

quint64 SetMeshDBDataCallback::getWrite()
{
    return write;
}

void SetMeshDBDataCallback::setWrite(const quint64 &value)
{
    write = value;
}

bool SetMeshDBDataCallback::getMd5Ok()
{
    return md5Ok;
}

void SetMeshDBDataCallback::setMd5Ok(bool value)
{
    md5Ok = value;
}
