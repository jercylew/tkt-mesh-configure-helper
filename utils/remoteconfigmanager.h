#ifndef REMOTECONFIGMANAGER_H
#define REMOTECONFIGMANAGER_H

#include <QObject>

class PrepareMeshModel;
class RemoteConfigSessionHolder;

class RemoteConfigManager : public QObject
{
    Q_OBJECT
public:
    explicit RemoteConfigManager(PrepareMeshModel *prepareMeshModel, QObject *parent = 0);

    RemoteConfigSessionHolder *remoteConfigSessionHolder();

private:
    PrepareMeshModel *m_prepareMeshModel;
    RemoteConfigSessionHolder *m_remoteConfigSessionHolder;

signals:

public slots:
};

#endif // REMOTECONFIGMANAGER_H
