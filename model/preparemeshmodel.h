#ifndef PREPAREMESHMODEL_H
#define PREPAREMESHMODEL_H

#include <QObject>

class Mesh;
class RemoteConfigManager;
class TKTMeshModel;

class PrepareMeshModel : public QObject
{
    Q_OBJECT
public:
    explicit PrepareMeshModel(Mesh *mesh, TKTMeshModel *tktMeshModel, QObject *parent = 0);

    Mesh *mesh();

    RemoteConfigManager *remoteConfigManager();

    TKTMeshModel *tktMeshModel();

private:
    Mesh *m_mesh;
    RemoteConfigManager *m_remoteConfigManager;
    TKTMeshModel *m_tktMeshModel;

signals:

public slots:
};

#endif // PREPAREMESHMODEL_H
