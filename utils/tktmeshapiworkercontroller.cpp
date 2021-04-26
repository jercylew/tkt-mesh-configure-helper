#include "tktmeshapiworkercontroller.h"
#include <QThread>
#include <QMessageBox>
#include "domain/tktmesh.h"
#include <QDebug>

TKTMeshApiWorkerController::TKTMeshApiWorkerController(TKTMesh *tktMesh, QObject *parent) : QObject(parent)
{
    this->m_tktMesh=tktMesh;
    m_worker=new TKTMeshApiWorker(tktMesh);
    m_worker->moveToThread(&m_workerThread);
    connect(&m_workerThread, SIGNAL(finished()), m_worker, SLOT(deleteLater()));
    connect(this, SIGNAL(asyncConnectHost()), m_worker, SLOT(connectHost()));
    connect(this, SIGNAL(asyncRequst(QJsonObject*)), m_worker, SLOT(asyncRequest(QJsonObject*)));
    connect(this, SIGNAL(asyncHeartBeat()), m_worker, SLOT(heartBeat()));
    connect(this, SIGNAL(asyncInitWorker()), m_worker, SLOT(initWorker()));

    connect(m_worker, SIGNAL(connectHostFinished(bool)), this, SLOT(doConnectHostFinished(bool)));
    connect(m_worker, SIGNAL(asyncResponse(QJsonObject*)), this, SLOT(doAsyncResponse(QJsonObject*)));
    connect(m_worker, SIGNAL(socketDisconnected()), this, SIGNAL(socketDisconnected()));
    m_workerThread.start();

    m_heartBeatTimer.setInterval(5000); //5 seconds
    connect(&m_heartBeatTimer, SIGNAL(timeout()), this, SLOT(doHeartBeatTimerout()));

    emit asyncInitWorker();
}

TKTMeshApiWorkerController::~TKTMeshApiWorkerController()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

void TKTMeshApiWorkerController::connectHost()
{
    emit asyncConnectHost();
}

void TKTMeshApiWorkerController::runAsyncTask(AsyncApiRequestTask *asyncTask)
{
    QJsonObject *requestJson=new QJsonObject(*asyncTask->requestJson());
    asyncTask->start();
    emit asyncRequst(requestJson);
}

void TKTMeshApiWorkerController::doAsyncResponse(QJsonObject *responseJson)
{
    QString topic=responseJson->value("topic").toString();
    if(topic=="response")
    {
        QString uuid=responseJson->value("uuid").toString();
        AsyncApiRequestTask *task=AsyncApiRequestTask::getTask(uuid);
        if(task!=NULL)
        {
            (*task->responseJson())=(*responseJson);
            task->finish(true);
        }
    }
    else if(topic=="report")
    {
        QString command=responseJson->value("command").toString();
        if(!command.isEmpty()){
            emit getReportData(command, responseJson);
        }
    }

    delete responseJson;
}

void TKTMeshApiWorkerController::doHeartBeatTimerout()
{
    emit asyncHeartBeat();
}

void TKTMeshApiWorkerController::doConnectHostFinished(bool ok)
{
    if(ok)
    {
        m_heartBeatTimer.start();
    }
    else
    {
        m_heartBeatTimer.stop();
    }
    emit connectHostFinished(ok);
}
