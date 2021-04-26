#include "tktmeshesmodel.h"
#include "domain/tktmesh.h"
#include "utils/tktmeshdiscoverhelper.h"

TKTMeshesModel::TKTMeshesModel(QObject *parent) : QObject(parent)
{
    m_tktMeshDiscoverHelper=new TKTMeshDiscoverHelper(this);
    connect(m_tktMeshDiscoverHelper, SIGNAL(discovered(TKTMesh*)),
            this, SLOT(doDiscovered(TKTMesh*)));
    connect(m_tktMeshDiscoverHelper, SIGNAL(scanProgress(int,int)),
            this, SIGNAL(scanProgress(int,int)));
}

void TKTMeshesModel::updateTKTMesh(TKTMesh *tktMesh)
{
    TKTMesh *oldTKTMesh=m_tktMeshMap.value(tktMesh->hostID());
    if(oldTKTMesh==NULL)
    {
        oldTKTMesh=new TKTMesh;
        m_tktMeshMap.insert(tktMesh->hostID(), oldTKTMesh);
        m_tktMeshList.push_back(oldTKTMesh);
    }
    oldTKTMesh->load(tktMesh);
    emit modelChanged();
    delete tktMesh;
}

QList<TKTMesh *> TKTMeshesModel::getAll()
{
    return this->m_tktMeshList;
}

TKTMesh *TKTMeshesModel::getByID(const QString &hostId)
{
    return this->m_tktMeshMap.value(hostId);
}

void TKTMeshesModel::scanTKTMesh(const QHostAddress &address1, const QHostAddress &address2, int waitMsec, bool clearAll)
{
    if(address1.toIPv4Address()>address2.toIPv4Address())
    {
        emit scanProgress(0, 0);
        return;
    }
    if(clearAll)
    {
        while(!m_tktMeshList.isEmpty())
        {
            delete m_tktMeshList.takeFirst();
        }
        m_tktMeshMap.clear();
        emit modelChanged();
    }
    m_tktMeshDiscoverHelper->scan(address1, address2, waitMsec);
}

void TKTMeshesModel::doDiscovered(TKTMesh *tktMesh)
{
    updateTKTMesh(tktMesh);
}
