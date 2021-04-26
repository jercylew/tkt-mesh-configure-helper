#include "nodegrouplistviewmodel.h"
#include <QDebug>
#include "nodegrouplistmodel.h"
#include "nodegroupsmodel.h"
#include "model/meshmodel.h"
#include "model/executersmodel.h"
#include "utils/meshsavingbox.h"

NodeGroupListViewModel::NodeGroupListViewModel(MeshModel *meshModel, NodeGroupsModel *nodeGroupsModel, QObject *parent):
    QAbstractListModel(parent),
    m_meshModel(meshModel)
{
    this->m_nodeGroupsModel=nodeGroupsModel;
    this->m_nodeGroupListModel=new NodeGroupListModel(this->m_nodeGroupsModel, meshModel->savingBox());
    connect(m_nodeGroupsModel, SIGNAL(groupDataChanged()), m_nodeGroupListModel, SLOT(doNodeGroupsModelDataChanged()));
    connect(m_nodeGroupsModel, SIGNAL(groupModelChanged()), m_nodeGroupListModel, SLOT(doNodeGroupsModelModelChanged()));
}

NodeGroupListViewModel::~NodeGroupListViewModel()
{
    if(m_nodeGroupListModel!=NULL)
    {
        delete m_nodeGroupListModel;
        m_nodeGroupListModel=NULL;
    }
}

int NodeGroupListViewModel::rowCount(const QModelIndex &parent) const
{
    return m_nodeGroupListModel->count();
}

int NodeGroupListViewModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant NodeGroupListViewModel::data(const QModelIndex &index, int role) const
{
    NodeGroup *nodeGroup=m_nodeGroupListModel->at(index.row());
    if(nodeGroup==NULL)
    {
        qDebug()<<"NodeGroupListViewModel::data, nodeGroup is NULL!";
        return QVariant();
    }
    else
    {
        switch (role) {
        case Qt::SizeHintRole:
            return QSize(0, 100);
        case Qt::UserRole:
            return nodeGroup->getId();
        case Qt::ToolTipRole:
            return tr("Global.GroupId:").append(QString(" %1").arg(nodeGroup->getId()));
        default:
            return QVariant();
        }
    }
    return QVariant();
}

void NodeGroupListViewModel::setSearchString(const QString &searchString)
{
    this->searchString=searchString;
}

QString NodeGroupListViewModel::getSearchString()
{
    return this->searchString;
}

NodeGroupListModel *NodeGroupListViewModel::nodeGroupListModel()
{
    return this->m_nodeGroupListModel;
}

