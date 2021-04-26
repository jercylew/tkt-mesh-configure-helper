#include "timelinecontrolpoliciesmodel.h"
#include "db/meshdbmanager.h"
#include "db/meshdbcaller.h"
#include <QDebug>
#include <QDateTime>

bool TimeLineControlPoliciesModel__timeLineControlPolicyLessThan(TimeLineControlPolicy *controlPolicy1, TimeLineControlPolicy *controlPolicy2)
{
    return controlPolicy1->id < controlPolicy2->id;
}

TimeLineControlPoliciesModel::TimeLineControlPoliciesModel(MeshModel *meshModel, QObject *parent)
    : QObject(parent)
{
    m_meshModel=meshModel;
}

TimeLineControlPolicy *TimeLineControlPoliciesModel::getById(int id)
{
    return m_controlPolicyMap.value(id);
}

void TimeLineControlPoliciesModel::updatePolicy(int controlPolicyId, const QString &name, const QString &color, const QString &description, int policySubId)
{
    TimeLineControlPolicy *controlPolicy=new TimeLineControlPolicy;
    controlPolicy->id=controlPolicyId;
    controlPolicy->name=name;
    controlPolicy->color=color;
    controlPolicy->description=description;
    controlPolicy->subId=policySubId;
    m_meshModel->dbManager()->dbCaller()->updateControlPolicy(controlPolicy);
}

QList<TimeLineControlPolicy *> TimeLineControlPoliciesModel::getSortedAllByGroupId(quint8 groupId)
{
    int len=m_controlPolicyList.size();
    QList<TimeLineControlPolicy *> resultList;
    for(int i=0; i<len; i++)
    {
        TimeLineControlPolicy *controlPolicy=m_controlPolicyList.at(i);
        if(controlPolicy->groupId==groupId)
        {
            resultList.push_back(controlPolicy);
        }
    }
    qSort(resultList.begin(), resultList.end(), TimeLineControlPoliciesModel__timeLineControlPolicyLessThan);
    return resultList;
}

void TimeLineControlPoliciesModel::doControlPoliciesChanged(QList<TimeLineControlPolicy *> *controlPolicyList)
{
    qDebug()<<"TimeLineControlPoliciesModel::doControlPoliciesChanged itemListList.size="<<controlPolicyList->size();
    while(!m_controlPolicyList.isEmpty())
    {
        delete m_controlPolicyList.takeFirst();
    }
    m_controlPolicyList.clear();
    m_controlPolicyMap.clear();

    int newLen=controlPolicyList->size();
    for(int i=0; i<newLen; i++)
    {
        TimeLineControlPolicy *controlPolicy=controlPolicyList->at(i);
        if(controlPolicy->subId==0)
        {
            controlPolicy->name=tr("TimeLineControlPoliciesModel.DefaultSchedulePolicy");
            controlPolicy->color="#888888";
            controlPolicy->description=tr("TimeLineControlPoliciesModel.DefaultSchedulePolicy");
        }
        m_controlPolicyList.push_back(controlPolicy);
        m_controlPolicyMap.insert(controlPolicy->id, controlPolicy);
    }
    emit controlPoliciesModelChanged();
}

void TimeLineControlPoliciesModel::doControlPolicyChanged(TimeLineControlPolicy *controlPolicy)
{
    TimeLineControlPolicy *oldPolicy=m_controlPolicyMap.value(controlPolicy->id);
    if(oldPolicy!=NULL)
    {
        oldPolicy->id=controlPolicy->id;
        oldPolicy->groupId=controlPolicy->groupId;
        oldPolicy->name=controlPolicy->name;
        oldPolicy->color=controlPolicy->color;
        oldPolicy->description=controlPolicy->description;
        oldPolicy->subId=controlPolicy->subId;
        if(oldPolicy->subId==0)
        {
            oldPolicy->name=tr("TimeLineControlPoliciesModel.DefaultSchedulePolicy");
            oldPolicy->color="#888888";
            oldPolicy->description=tr("TimeLineControlPoliciesModel.DefaultSchedulePolicy");
        }
    }
    delete controlPolicy;
    emit controlPoliciesDataChanged();
}

int TimeLineControlPoliciesModel::getNextSubId(quint8 groupId)
{
    int subId=0;
    for(int i=1; i<=50; i++)
    {
        int id=((groupId&0xFF)*100+i);
        if(!m_controlPolicyMap.contains(id))
        {
            subId=i;
            break;
        }
    }
    return subId;
}

