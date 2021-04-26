#include "tktmeshupdatelistmodel.h"
#include "tktmeshesupdatemodel.h"
#include "domain/tktmesh.h"

TKTMeshUpdateListModel::TKTMeshUpdateListModel(TKTMeshesUpdateModel *tktMeshesUpdateModel, QObject *parent) : QObject(parent)
{
    this->m_tktMeshesUpdateModel=tktMeshesUpdateModel;
    connect(m_tktMeshesUpdateModel, SIGNAL(dataChanged()), this, SLOT(doDataChanged()));
    connect(m_tktMeshesUpdateModel, SIGNAL(modelChanged()), this, SLOT(doModelChanged()));
}

TKTMesh *TKTMeshUpdateListModel::at(int index)
{
    return m_tktMeshList.at(index);
}

int TKTMeshUpdateListModel::count()
{
    return m_tktMeshList.size();
}

void TKTMeshUpdateListModel::refreshList()
{
    m_tktMeshList=m_tktMeshesUpdateModel->getAll();
}

void TKTMeshUpdateListModel::doDataChanged()
{
    emit dataChanged();
}

void TKTMeshUpdateListModel::doModelChanged()
{
    refreshList();
    emit modelChanged();
}
