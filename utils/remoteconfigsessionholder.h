#ifndef REMOTECONFIGSESSIONHOLDER_H
#define REMOTECONFIGSESSIONHOLDER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class PrepareMeshModel;

class RemoteConfigSessionHolder : public QObject
{
    Q_OBJECT
public:
    explicit RemoteConfigSessionHolder(PrepareMeshModel *prepareMeshModel, QObject *parent = 0);
    ~RemoteConfigSessionHolder();

    void startKeepCommandReportTimer();
    void stopKeepCommandReportTimer();

    void writeCommandToTKTMesh(const QByteArray &cmd);

private:
    PrepareMeshModel *m_prepareMeshModel;
    QTimer m_keepCommandReportTimer;

signals:
    void readADataFrame(const QByteArray &dataFrame);

public slots:
    void doKeepCommandReportTimerTimeout();
    void doGetReportData(const QString &command, QJsonObject *reportJson);
};

#endif // REMOTECONFIGSESSIONHOLDER_H
