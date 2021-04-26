#ifndef GETMESHDBINFOCALLBACK_H
#define GETMESHDBINFOCALLBACK_H

#include "utils/asyncapirequesttask.h"
#include <QObject>

class AbstractCallbackInvoker;
class PrepareMeshModel;

class GetMeshDBInfoCallback : public AsyncApiRequestTaskCallback
{
    Q_OBJECT
public:
    GetMeshDBInfoCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, PrepareMeshModel *prepareMeshModel);

    void taskStarted();
    void taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask);

    QString getMd5();
    void setMd5(const QString &value);

    quint64 getTotal();
    void setTotal(const quint64 &value);

private:
    PrepareMeshModel *m_prepareMeshModel;
    QString md5;
    quint64 total;

signals:
    void getMeshDBInfoFinished(bool ok);
};

#endif // GETMESHDBINFOCALLBACK_H
