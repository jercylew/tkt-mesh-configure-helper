#include "meshlistmodel.h"
#include "model/meshesmodel.h"

MeshListModel::MeshListModel(MeshesModel *meshesModel, QObject *parent) : QObject(parent)
{
    this->m_meshesModel=meshesModel;
    connect(m_meshesModel, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));
    connect(m_meshesModel, SIGNAL(modelChanged()), this, SLOT(doModelChanged()));
    refreshList();
}

int MeshListModel::count()
{
    return m_meshList.size();
}

Mesh *MeshListModel::at(int index)
{
    return m_meshList.at(index);
}

void MeshListModel::refreshList()
{
    m_meshList=m_meshesModel->getAll();
}

void MeshListModel::doModelChanged()
{
    refreshList();
    emit modelChanged();
}
