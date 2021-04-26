#ifndef MESHLOGMANAGER_H
#define MESHLOGMANAGER_H

#include <QObject>
#include <QList>
#include <QTimer>

class MeshModel;

class MeshLogManager : public QObject
{
    Q_OBJECT
public:
    explicit MeshLogManager(MeshModel *meshModel, QObject *parent = 0);

    void pushLog(const QString &log, bool debugInfo=false);
    QList<QString> getAllLog();
    void clearLog();

    QString logFolder();
    void setLogFolder(const QString &logFolder);

private:
    QList<QString> m_logList;
    QList<QString> m_bufferedLog;
    MeshModel *m_meshModel;

    QString m_logFolder;
    QTimer m_logSaveTimer;

signals:

public slots:
    void doLogSaveTimerout();
};

#endif // MESHLOGMANAGER_H
