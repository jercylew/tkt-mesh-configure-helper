#ifndef ASYNCAPIREQUESTTASK_H
#define ASYNCAPIREQUESTTASK_H

#include <QObject>
#include <QTimer>
#include "callback/abstractcallbackalerter.h"
#include "callback/abstractcallbackinvoker.h"

class AsyncApiRequestTask;
class AbstractCallbackInvoker;
class AbstractCallbackAlerter;

class AsyncApiRequestTaskCallback : public QObject{
public:
    AsyncApiRequestTaskCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter);

    virtual void taskStarted()=0;
    virtual void taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask)=0;

protected:
    AbstractCallbackAlerter *alerter();
    AbstractCallbackInvoker *invoker();

private:
    AbstractCallbackAlerter *m_alerter;
    AbstractCallbackInvoker *m_invoker;
};

class AsyncApiRequestTask : public QObject
{
    Q_OBJECT
public:
    explicit AsyncApiRequestTask(AsyncApiRequestTaskCallback *callback, QObject *parent = 0);
    ~AsyncApiRequestTask();

    void start();
    void finish(bool isNormalFinish);
    void destroy();

    QString requestUUID();
    QJsonObject *requestJson();
    QJsonObject *responseJson();

    static AsyncApiRequestTask *getTask(const QString &uuid);

private:
    QString m_requestUUID;
    AsyncApiRequestTaskCallback *m_callback;
    QJsonObject *m_requestJson;
    QJsonObject *m_responseJson;

    QTimer m_timeoutTimer;

    static QMap<QString, AsyncApiRequestTask*> m_asyncTaskMap;

private slots:
    void doTimeout();

signals:

public slots:
};

#endif // ASYNCAPIREQUESTTASK_H
