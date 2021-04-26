#include "meshnodelistmodel.h"
#include "model/meshnodesmodel.h"

MeshNodeListModel::MeshNodeListModel(MeshNodesModel *meshNodesModel, QObject *parent) : QObject(parent)
{
    this->m_meshNodesModel=meshNodesModel;
    connect(m_meshNodesModel, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));
    connect(m_meshNodesModel, SIGNAL(modelChanged()), this, SLOT(doModelChanged()));
    refreshList();
}

int MeshNodeListModel::count()
{
    return m_nodeList.size();
}

MeshDeviceNode *MeshNodeListModel::at(int index)
{
    return m_nodeList.at(index);
}

void MeshNodeListModel::refreshList()
{
    m_nodeList=m_meshNodesModel->getAll();
}

void MeshNodeListModel::doModelChanged()
{
    refreshList();
    emit modelChanged();
}
