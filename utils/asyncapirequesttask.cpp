#include "asyncapirequesttask.h"
#include "tools.h"
#include <QDebug>

QMap<QString, AsyncApiRequestTask*> AsyncApiRequestTask::m_asyncTaskMap=QMap<QString, AsyncApiRequestTask*>();

AsyncApiRequestTask::AsyncApiRequestTask(AsyncApiRequestTaskCallback *callback, QObject *parent) : QObject(parent)
{
    this->m_callback=callback;
    m_requestUUID=Tools::getRandomUUID();
    m_requestJson=new QJsonObject;
    m_requestJson->insert("uuid", m_requestUUID);
    m_responseJson=new QJsonObject;
    m_responseJson->insert("uuid", m_requestUUID);
    m_timeoutTimer.setInterval(10*1000); //10 seconds
    m_timeoutTimer.setSingleShot(true);
    connect(&m_timeoutTimer, SIGNAL(timeout()), this, SLOT(doTimeout()));
}

AsyncApiRequestTask::~AsyncApiRequestTask()
{
    m_timeoutTimer.stop();
    delete m_requestJson;
    delete m_responseJson;
    if(m_callback!=NULL)
    {
        m_callback->deleteLater();
    }
}

void AsyncApiRequestTask::start()
{
    m_asyncTaskMap.insert(this->m_requestUUID, this);
    m_timeoutTimer.start();
    if(m_callback!=NULL)
    {
        m_callback->taskStarted();
    }
}

void AsyncApiRequestTask::finish(bool isNormalFinish)
{
    m_asyncTaskMap.remove(this->m_requestUUID);
    m_timeoutTimer.stop();
    if(m_callback!=NULL)
    {
        m_callback->taskFinished(isNormalFinish, this);
    }
}

void AsyncApiRequestTask::destroy()
{
    m_timeoutTimer.stop();
    this->deleteLater();
}

QString AsyncApiRequestTask::requestUUID()
{
    return this->m_requestUUID;
}

QJsonObject *AsyncApiRequestTask::requestJson()
{
    return this->m_requestJson;
}

QJsonObject *AsyncApiRequestTask::responseJson()
{
    return this->m_responseJson;
}

AsyncApiRequestTask *AsyncApiRequestTask::getTask(const QString &uuid)
{
    return m_asyncTaskMap.value(uuid);
}

void AsyncApiRequestTask::doTimeout()
{
    qDebug()<<"request command="<<this->m_requestJson->value("command").toString()<<", uuid="<<this->m_requestUUID<<" timeout!";
    finish(false);
}

AbstractCallbackAlerter *AsyncApiRequestTaskCallback::alerter()
{
    return m_alerter;
}

AbstractCallbackInvoker *AsyncApiRequestTaskCallback::invoker()
{
    return m_invoker;
}

AsyncApiRequestTaskCallback::AsyncApiRequestTaskCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter)
{
    this->m_invoker=invoker;
    this->m_alerter=alerter;
}
