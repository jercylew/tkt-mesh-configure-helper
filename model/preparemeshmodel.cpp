#include "preparemeshmodel.h"
#include "utils/remoteconfigmanager.h"

PrepareMeshModel::PrepareMeshModel(Mesh *mesh, TKTMeshModel *tktMeshModel, QObject *parent) : QObject(parent)
{
    this->m_mesh=mesh;
    m_tktMeshModel=tktMeshModel;
    m_remoteConfigManager=new RemoteConfigManager(this, this);
}

Mesh *PrepareMeshModel::mesh()
{
    return this->m_mesh;
}

RemoteConfigManager *PrepareMeshModel::remoteConfigManager()
{
    return this->m_remoteConfigManager;
}

TKTMeshModel *PrepareMeshModel::tktMeshModel()
{
    return this->m_tktMeshModel;
}
