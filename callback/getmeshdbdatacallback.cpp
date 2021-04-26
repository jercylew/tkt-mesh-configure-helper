#include "getmeshdbdatacallback.h"
#include "model/meshmodel.h"
#include "utils/remoteconfigmanager.h"
#include "utils/remoteconfigsessionholder.h"
#include <QDebug>
#include "model/preparemeshmodel.h"

GetMeshDBDataCallback::GetMeshDBDataCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, PrepareMeshModel *prepareMeshModel)
   : AsyncApiRequestTaskCallback(invoker, alerter)
{
    this->m_prepareMeshModel=prepareMeshModel;
}

void GetMeshDBDataCallback::taskStarted()
{
}

void GetMeshDBDataCallback::taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask)
{
    if(ok && asyncApiRequestTask->responseJson()->value("result").toString()=="success")
    {
        QJsonObject dataJson=asyncApiRequestTask->responseJson()->value("data").toObject();

        quint64 offset=dataJson.value("offset").toString().toULongLong();
        quint64 length=dataJson.value("length").toString().toULongLong();
        QString dbDataBase64=dataJson.value("db_data").toString();

        this->offset=offset;
        this->length=length;
        this->dbData=QByteArray::fromBase64(dbDataBase64.toLatin1());

        emit getMeshDBDataFinished(true);
    }
    else
    {
        emit getMeshDBDataFinished(false);
    }
    asyncApiRequestTask->destroy();
}

quint64 GetMeshDBDataCallback::getOffset()
{
    return offset;
}

void GetMeshDBDataCallback::setOffset(const quint64 &value)
{
    offset = value;
}

quint64 GetMeshDBDataCallback::getLength()
{
    return length;
}

void GetMeshDBDataCallback::setLength(const quint64 &value)
{
    length = value;
}

QByteArray GetMeshDBDataCallback::getDbData()
{
    return dbData;
}

void GetMeshDBDataCallback::setDbData(const QByteArray &value)
{
    dbData = value;
}
