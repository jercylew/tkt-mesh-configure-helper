#include "meshnodesmodel.h"
#include "domain/meshdevicenode.h"

MeshNodesModel::MeshNodesModel(QObject *parent) : QObject(parent)
{

}

MeshDeviceNode *MeshNodesModel::getByNodeID(const QString &nodeId)
{
    return m_nodeMap.value(nodeId);
}

void MeshNodesModel::updateNode(MeshDeviceNode *node)
{
    MeshDeviceNode *oldNode=m_nodeMap.value(node->nodeId());
    if(oldNode==NULL)
    {
        oldNode=node->copy();
        m_nodeMap.insert(oldNode->nodeId(), oldNode);
        m_nodeList.push_back(oldNode);
        emit modelChanged();
    }
    else
    {
        oldNode->load(node);
        emit dataChanged();
    }
}

void MeshNodesModel::updateNodes(QList<MeshDeviceNode *> nodeList)
{
    for(int i=0; i<nodeList.size(); i++)
    {
        MeshDeviceNode *node=nodeList.at(i);
        MeshDeviceNode *oldNode=m_nodeMap.value(node->nodeId());
        if(oldNode==NULL)
        {
            oldNode=node->copy();
            m_nodeMap.insert(oldNode->nodeId(), oldNode);
            m_nodeList.push_back(oldNode);
        }
        else
        {
            oldNode->load(node);
        }
    }
    emit modelChanged();
}

QList<MeshDeviceNode *> MeshNodesModel::getAll()
{
    return this->m_nodeList;
}
