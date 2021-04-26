#ifndef TKTMESHAPIWORKER_H
#define TKTMESHAPIWORKER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QTimer>

class TKTMesh;

class TKTMeshApiWorker : public QObject
{
    Q_OBJECT
public:
    explicit TKTMeshApiWorker(TKTMesh *tktMesh, QObject *parent = 0);
    ~TKTMeshApiWorker();

public slots:
    void initWorker();
    void connectHost();
    void asyncRequest(QJsonObject *requestJson);
    void heartBeat();
    void doReadyRead();

private:
    TKTMesh *m_tktMesh;
    QTcpSocket *m_tcpSocket;
    QByteArray m_dataBuffer;
    QTimer *m_heartBeatDeadTimer;
    void resetHeartBeatDeadTimer();

signals:
    void connectHostFinished(bool ok);
    void asyncResponse(QJsonObject *responseJson);
    void socketDisconnected();

private slots:
    void doHeartBeatDeadTimerout();

public slots:
};

#endif // TKTMESHAPIWORKER_H
