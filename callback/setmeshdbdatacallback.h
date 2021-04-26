#ifndef SETMESHDBDATACALLBACK_H
#define SETMESHDBDATACALLBACK_H

#include "utils/asyncapirequesttask.h"
#include <QObject>

class AbstractCallbackInvoker;
class PrepareMeshModel;

class SetMeshDBDataCallback : public AsyncApiRequestTaskCallback
{
    Q_OBJECT
public:
    SetMeshDBDataCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, PrepareMeshModel *prepareMeshModel);

    void taskStarted();
    void taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask);

    quint64 getTotal();
    void setTotal(const quint64 &value);

    quint64 getWrite();
    void setWrite(const quint64 &value);

    bool getMd5Ok();
    void setMd5Ok(bool value);

private:
    PrepareMeshModel *m_prepareMeshModel;

    quint64 total;
    quint64 write;
    bool md5Ok;

signals:
    void setMeshDBDataFinished(bool ok);
};

#endif // SETMESHDBDATACALLBACK_H
