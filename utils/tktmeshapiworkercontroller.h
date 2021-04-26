#ifndef TKTMESHAPIWOKERCONTROLLER_H
#define TKTMESHAPIWOKERCONTROLLER_H

#include <QObject>
#include "tktmeshapiworker.h"
#include <QThread>
#include "asyncapirequesttask.h"
#include <QTimer>

class TKTMesh;

class TKTMeshApiWorkerController : public QObject
{
    Q_OBJECT
public:
    explicit TKTMeshApiWorkerController(TKTMesh *tktMesh, QObject *parent = 0);
    ~TKTMeshApiWorkerController();

    void connectHost();
    void runAsyncTask(AsyncApiRequestTask *asyncTask);

private:
    QThread m_workerThread;
    TKTMeshApiWorker *m_worker;
    QTimer m_heartBeatTimer;
    TKTMesh *m_tktMesh;

signals:
    void asyncInitWorker();
    void asyncConnectHost();
    void asyncRequst(QJsonObject *requestJson);
    void asyncHeartBeat();
    void connectHostFinished(bool ok);
    void error(const QString &error);
    void socketDisconnected();
    void getReportData(const QString &command, QJsonObject *reportJson);

public slots:
    void doAsyncResponse(QJsonObject *responseJson);
    void doHeartBeatTimerout();
    void doConnectHostFinished(bool ok);
};

#endif // TKTMESHAPIWOKERCONTROLLER_H
