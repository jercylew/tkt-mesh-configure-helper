#include "tktmeshlistmodel.h"
#include "tktmeshesmodel.h"
#include "domain/tktmesh.h"

TKTMeshListModel::TKTMeshListModel(TKTMeshesModel *tktMeshesModel, QObject *parent) : QObject(parent)
{
    this->m_tktMeshesModel=tktMeshesModel;
    connect(tktMeshesModel, SIGNAL(dataChanged()), this, SLOT(doDataChanged()));
    connect(tktMeshesModel, SIGNAL(modelChanged()), this, SLOT(doModelChanged()));
}

TKTMesh *TKTMeshListModel::at(int index)
{
    return m_tktMeshList.at(index);
}

int TKTMeshListModel::count()
{
    return m_tktMeshList.size();
}

void TKTMeshListModel::refreshList()
{
    m_tktMeshList=m_tktMeshesModel->getAll();
}

void TKTMeshListModel::doDataChanged()
{
    emit dataChanged();
}

void TKTMeshListModel::doModelChanged()
{
    refreshList();
    emit modelChanged();
}
