#include "tktmeshconnectionholder.h"
#include "domain/tktmesh.h"
#include "utils/tools.h"
#include <QHostAddress>

TKTMeshConnectionHolder::TKTMeshConnectionHolder(TKTMesh *tktMesh, QObject *parent) : QObject(parent)
{
    this->m_tktMesh=tktMesh;
    m_apiWorkerController=new TKTMeshApiWorkerController(tktMesh, this);
}

TKTMeshConnectionHolder::~TKTMeshConnectionHolder()
{
}

TKTMeshApiWorkerController *TKTMeshConnectionHolder::apiWorkerController()
{
    return this->m_apiWorkerController;
}

