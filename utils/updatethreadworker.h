#ifndef UPDATETHREADWORKER_H
#define UPDATETHREADWORKER_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>

class UpdateThreadWorker : public QObject
{
    Q_OBJECT
public:
    explicit UpdateThreadWorker(const QString &hostId, const QString &ip, const QString &updateFilePath, QThread *workerThread, QObject *parent = 0);
    ~UpdateThreadWorker();

    QThread *workerThread() const;

public slots:
    void startUpdate();

private slots:
    void doTCPSocketConnected();
    void doTCPSocketDisconnected();
    void doTCPSocketReadyRead();
    void doTCPSocketBytesWritten(qint64 bytes);

private:
    QTcpSocket *m_tcpSocket;
    QString m_hostId;
    QString m_ip;
    QString m_updateFilePath;
    qint64 m_writeFileSize;
    QFile *m_updateFile;
    QByteArray m_readDataBuffer;
    QThread *m_workerThread;
    bool m_workIsFinished;

signals:
    void updateStarted(const QString &hostId);
    void updateProgressReport(const QString &hostId, int progress);
    void updateFinished(const QString &hostId, bool ok);

public slots:
};

#endif // UPDATETHREADWORKER_H
