#include "nodegrouplistmodel.h"
#include "domain/nodegroup.h"
#include "model/nodegroupsmodel.h"
#include <QDebug>
#include "utils/meshsavingbox.h"

bool NodeGroupListModel__groupIDLessThan(NodeGroup *group1, NodeGroup *group2)
{
    return group1->getId()<group2->getId();
}

bool NodeGroupListModel__groupNameLessThan(NodeGroup *group1, NodeGroup *group2)
{
    return group1->getName() < group2->getName();
}

NodeGroupListModel::NodeGroupListModel(NodeGroupsModel *nodeGroupsModel, MeshSavingBox *savingBox, QObject *parent)
    : QObject(parent),
      m_nodeGroupsModel(nodeGroupsModel)
{
    this->m_savingBox=savingBox;
}

void NodeGroupListModel::init()
{
    refreshList();
}

int NodeGroupListModel::count()
{
    return m_nodeGroupList.size();
}

NodeGroup *NodeGroupListModel::at(int index)
{
    if(m_nodeGroupList.size()==0)
    {
        return NULL;
    }
    else if(index<m_nodeGroupList.size())
    {
        return m_nodeGroupList.at(index);
    }
    else
    {
        return NULL;
    }
}

void NodeGroupListModel::setSearchString(const QString &searchString)
{
    if(this->m_searchString!=searchString)
    {
        this->m_searchString=searchString;
        refreshList();
    }
}

QString NodeGroupListModel::searchString()
{
    return this->m_searchString;
}

MeshRuntimeParameterBox::NodeGroupOrderMethod NodeGroupListModel::orderMethod()
{
    return m_savingBox->runtimeParameterBox()->nodeGroupOrderMethod();
}

void NodeGroupListModel::setOrderMethod(MeshRuntimeParameterBox::NodeGroupOrderMethod orderMethod)
{
    if(m_savingBox->runtimeParameterBox()->nodeGroupOrderMethod()!=orderMethod)
    {
        m_savingBox->runtimeParameterBox()->setNodeGroupOrderMethod(orderMethod);
        refreshList();
    }
}

void NodeGroupListModel::refreshList()
{
    m_nodeGroupList.clear();
    QList<NodeGroup*> tmpList=m_nodeGroupsModel->getAll();
    int tmpLen=tmpList.size();
    if(m_searchString.isEmpty())
    {
        m_nodeGroupList=tmpList;
    }
    else
    {
        for(int i=0; i<tmpLen; i++)
        {
            NodeGroup *nodeGroup=tmpList.at(i);
            if(nodeGroup->getName().toLower().contains(m_searchString.toLower()))
            {
                m_nodeGroupList.push_back(nodeGroup);
            }
        }
    }

    if(m_savingBox->runtimeParameterBox()->nodeGroupOrderMethod()==MeshRuntimeParameterBox::NodeGroupOrderByGroupID)
    {
        qSort(m_nodeGroupList.begin(), m_nodeGroupList.end(), NodeGroupListModel__groupIDLessThan);
    }
    else if(m_savingBox->runtimeParameterBox()->nodeGroupOrderMethod()==MeshRuntimeParameterBox::NodeGroupOrderByGroupName)
    {
        qSort(m_nodeGroupList.begin(), m_nodeGroupList.end(), NodeGroupListModel__groupNameLessThan);
    }

    emit modelChanged();
}

void NodeGroupListModel::doNodeGroupsModelDataChanged()
{
    emit dataChanged();
}

void NodeGroupListModel::doNodeGroupsModelModelChanged()
{
    refreshList();
}
