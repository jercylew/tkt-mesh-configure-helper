#include "tktmeshmodel.h"
#include "utils/tktmeshconnectionholder.h"
#include "utils/tktmeshapiworkercontroller.h"

TKTMeshModel::TKTMeshModel(TKTMesh *tktMesh, QObject *parent) : QObject(parent)
{
    this->m_tktMesh=tktMesh;
    m_tktMeshConnectionHolder=new TKTMeshConnectionHolder(this->m_tktMesh, this);
}

TKTMeshModel::~TKTMeshModel()
{

}

TKTMeshConnectionHolder *TKTMeshModel::tktMeshConnectionHolder()
{
    return this->m_tktMeshConnectionHolder;
}

TKTMesh *TKTMeshModel::tktMesh()
{
    return this->m_tktMesh;
}
