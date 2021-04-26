#include "timelinecontrolpolicymodel.h"
#include "timelinecontrolitemsmodel.h"
#include "utils/timelinecontrolmanager.h"
#include <QDebug>

TimeLineControlPolicyModel::TimeLineControlPolicyModel(MeshModel *meshModel, quint8 groupId, int controlPolicySubId, QObject *parent)
    : QObject(parent)
{
    this->m_meshModel=meshModel;
    this->m_groupId=groupId;
    this->m_controlPolicySubId=controlPolicySubId;
    connect(meshModel->timeLineControlManager()->getTimeLineControlItemsModel(), SIGNAL(controlItemsModelChanged()),
            this, SLOT(doModelChanged()));
    connect(meshModel->timeLineControlManager()->getTimeLineControlItemsModel(), SIGNAL(controlItemsDataChanged()),
            this, SLOT(doDataChanged()));
}

void TimeLineControlPolicyModel::init()
{
    refreshList();
}

int TimeLineControlPolicyModel::count()
{
    return m_timeLineControlItemList.size();
}

TimeLineControlItem *TimeLineControlPolicyModel::at(int index)
{
    if(m_timeLineControlItemList.size()==0)
    {
        return NULL;
    }
    else if(index<m_timeLineControlItemList.size())
    {
        return m_timeLineControlItemList.at(index);
    }
    else
    {
        return NULL;
    }
}

void TimeLineControlPolicyModel::refreshList()
{
    m_timeLineControlItemList.clear();
    QList<TimeLineControlItem*> itemList=m_meshModel->timeLineControlManager()->getTimeLineControlItemsModel()->getTimeLineControlItemList(this->m_groupId, this->m_controlPolicySubId);
    if(!itemList.isEmpty())
    {
        for(int i=0; i<itemList.size(); i++)
        {
            m_timeLineControlItemList.push_back(itemList.at(i));
        }
    }

    emit modelChanged();
}

void TimeLineControlPolicyModel::doModelChanged()
{
    refreshList();
    emit modelChanged();
}

void TimeLineControlPolicyModel::doDataChanged()
{
    emit dataChanged();
}
