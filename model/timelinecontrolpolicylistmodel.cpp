#include "timelinecontrolpolicylistmodel.h"
#include "utils/timelinecontrolmanager.h"
#include <QDebug>

TimeLineControlPolicyListModel::TimeLineControlPolicyListModel(MeshModel *meshModel, quint8 groupId, QObject *parent) : QObject(parent)
{
    this->m_groupId=groupId;
    this->m_meshModel=meshModel;
    connect(meshModel->timeLineControlManager()->timeLineControlPoliciesModel(), SIGNAL(controlPoliciesModelChanged()),
            this, SLOT(refreshList()));
    connect(meshModel->timeLineControlManager()->timeLineControlPoliciesModel(), SIGNAL(controlPoliciesDataChanged()),
            this, SIGNAL(dataChanged()));
}

int TimeLineControlPolicyListModel::count()
{
    return this->m_timeLineControlPolicyList.size();
}

void TimeLineControlPolicyListModel::init()
{
    refreshList();
}

TimeLineControlPolicy *TimeLineControlPolicyListModel::at(int index)
{
    if(m_timeLineControlPolicyList.size()==0)
    {
        return NULL;
    }
    else if(index<m_timeLineControlPolicyList.size())
    {
        return m_timeLineControlPolicyList.at(index);
    }
    else
    {
        return NULL;
    }
}

void TimeLineControlPolicyListModel::refreshList()
{
    m_timeLineControlPolicyList=m_meshModel->timeLineControlManager()->timeLineControlPoliciesModel()->getSortedAllByGroupId(m_groupId);
    emit modelChanged();
}
