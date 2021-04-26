#include "nodegroupsmodel.h"
#include <QAction>
#include "db/meshdbmanager.h"
#include <QDebug>
#include "model/meshmodel.h"
#include "db/meshdbcaller.h"

NodeGroupsModel::NodeGroupsModel(MeshModel *meshModel, QObject *parent)
    : QObject(parent),
      m_meshModel(meshModel)
{

}

void NodeGroupsModel::addGroup(quint8 groupId, const QString &groupName, const QColor &groupColor, NodeGroup::GroupType groupType)
{
    if(nodeGroupMap.contains(groupId))
    {
        return;
    }
    NodeGroup *nodeGroup=new NodeGroup;
    nodeGroup->setId(groupId);
    nodeGroup->setName(groupName);
    nodeGroup->setColor(groupColor.name());
    nodeGroup->setGroupType(groupType);
    m_meshModel->dbManager()->dbCaller()->addNodeGroup(nodeGroup);
    qDebug()<<QString("NodeGroupsModel::addGroup, operation success groupId=%1").arg(groupId);
}

void NodeGroupsModel::removeGroup(quint8 groupId)
{
    m_meshModel->dbManager()->dbCaller()->removeNodeGroup(groupId);
}

int NodeGroupsModel::count()
{
    return nodeGroupList.size();
}

NodeGroup *NodeGroupsModel::at(int index)
{
    if(index>=0 && index<nodeGroupList.size())
    {
        return nodeGroupList.at(index);
    }
    else
    {
        return NULL;
    }
}

NodeGroup *NodeGroupsModel::getNodeGroup(quint8 groupId)
{
    return nodeGroupMap.value(groupId);
}

QList<QColor> NodeGroupsModel::getColorList(const QList<quint8> groupList)
{
    QList<QColor> colorList;
    for(int i=0; i<groupList.size(); i++)
    {
        NodeGroup *group=nodeGroupMap.value(groupList.at(i));
        if(group!=NULL)
        {
            colorList.append(group->getColor());
        }
    }
    return colorList;
}

QList<QAction *> NodeGroupsModel::getNodeGroupActionList()
{
    return this->nodeGroupActionList;
}

QList<NodeGroup *> NodeGroupsModel::getNodeGroupList()
{
    return this->nodeGroupList;
}

int NodeGroupsModel::countOfSearchString(const QString &searchString)
{
    if(searchString.isEmpty())
    {
        return nodeGroupList.size();
    }
    else
    {
        int len=nodeGroupList.size();
        int size=0;
        for(int i=0; i<len; i++)
        {
            NodeGroup *nodeGroup=nodeGroupList.at(i);
            if(nodeGroup->getName().toLower().contains(searchString))
            {
                size++;
            }
        }
        return size;
    }
}

NodeGroup *NodeGroupsModel::atOfSearchString(const QString &searchString, int index)
{
    if(searchString.isEmpty())
    {
        if(index<nodeGroupList.size())
        {
            return nodeGroupList.at(index);
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        QList<NodeGroup*> groupList;
        for(NodeGroup *group : nodeGroupList)
        {
            if(group->getName().toLower().contains(searchString))
            {
                groupList.push_back(group);
            }
        }
        if(index<groupList.size())
        {
            return groupList.at(index);
        }
        else
        {
            return NULL;
        }
    }
}

QList<NodeGroup *> NodeGroupsModel::getAll()
{
    return this->nodeGroupList;
}

NodeGroup::ScheduleType NodeGroupsModel::getGroupScheduleType(quint8 groupId)
{
    NodeGroup *nodeGroup=nodeGroupMap.value(groupId);
    NodeGroup::ScheduleType scheduleType=NodeGroup::ScheduleOff;
    if(nodeGroup!=NULL)
    {
        scheduleType=nodeGroup->getScheduleType();
    }
    return scheduleType;
}

void NodeGroupsModel::setGroupScheduleType(quint8 groupId, NodeGroup::ScheduleType scheduleType)
{
    NodeGroup *oldGroup=this->getNodeGroup(groupId);
    if(oldGroup==NULL)
    {
        return;
    }
    NodeGroup *newGroup=oldGroup->copyAll();
    newGroup->setScheduleType(scheduleType);
    m_meshModel->dbManager()->dbCaller()->updateNodeGroupsScheduleData(newGroup);
}

void NodeGroupsModel::onExecuterGroupInfoChanged()
{
    emit groupDataChanged();
}


void NodeGroupsModel::doGroupActionTriggered(bool checked)
{
    QAction *action=static_cast<QAction*>(sender());
    if(action==NULL)
    {
        return;
    }
    quint8 groupId=(quint8)(action->property("group_id").toInt());
    emit groupActionTriggered(action->isChecked(), groupId);
}

void NodeGroupsModel::doNodeGroupsChanged(QList<NodeGroup *> *groupList)
{
    qDebug()<<"NodeGroupsModel::doNodeGroupsChanged groupList.size="<<groupList->size();
    int oldGroupListLen=nodeGroupList.size();
    for(int i=0; i<oldGroupListLen; i++)
    {
        delete nodeGroupList.at(i);
    }
    nodeGroupList.clear();
    nodeGroupMap.clear();

    int oldActionListLen=nodeGroupActionList.size();
    for(int i=0; i<oldActionListLen; i++)
    {
        nodeGroupActionList.at(i)->deleteLater();
    }
    nodeGroupActionList.clear();
    nodeGroupActionMap.clear();

    int len=groupList->size();
    for(int i=0; i<len; i++)
    {
        NodeGroup *nodeGroupNew=groupList->at(i);
        nodeGroupMap.insert(nodeGroupNew->getId(), nodeGroupNew);
        nodeGroupList.append(nodeGroupNew);

        QAction *action=new QAction;
        action->setCheckable(true);
        connect(action, SIGNAL(triggered(bool)), this, SLOT(doGroupActionTriggered(bool)));
        action->setText(nodeGroupNew->getName());
        action->setProperty("group_id", nodeGroupNew->getId());
        nodeGroupActionMap.insert(nodeGroupNew->getId(), action);
        nodeGroupActionList.append(action);
    }
    delete groupList;
    emit groupModelChanged();
}

