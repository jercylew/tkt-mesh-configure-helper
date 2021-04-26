#ifndef GETMESHDBDATACALLBACK_H
#define GETMESHDBDATACALLBACK_H

#include "utils/asyncapirequesttask.h"
#include <QObject>

class AbstractCallbackInvoker;
class PrepareMeshModel;

class GetMeshDBDataCallback : public AsyncApiRequestTaskCallback
{
    Q_OBJECT
public:
    GetMeshDBDataCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, PrepareMeshModel *prepareMeshModel);

    void taskStarted();
    void taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask);

    quint64 getOffset();
    void setOffset(const quint64 &value);

    quint64 getLength();
    void setLength(const quint64 &value);

    QByteArray getDbData();
    void setDbData(const QByteArray &value);

private:
    PrepareMeshModel *m_prepareMeshModel;

    quint64 offset;
    quint64 length;
    QByteArray dbData;

signals:
    void getMeshDBDataFinished(bool ok);
};

#endif // GETMESHDBDATACALLBACK_H
