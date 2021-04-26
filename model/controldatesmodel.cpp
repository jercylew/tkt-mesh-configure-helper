#include "controldatesmodel.h"
#include <QDate>
#include <QDebug>
#include "meshmodel.h"
#include "db/meshdbmanager.h"
#include "db/meshdbcaller.h"
#include "model/nodegroupsmodel.h"

ControlDatesModel::ControlDatesModel(MeshModel *meshModel, QObject *parent)
    : QObject(parent),
      m_meshModel(meshModel)
{

}

ControlDate *ControlDatesModel::getById(const QString &id)
{
    return m_controlDateMap.value(id);
}

int ControlDatesModel::getTodayPolicyId(quint8 groupId)
{
    NodeGroup *nodeGroup=m_meshModel->nodeGroupsModel()->getNodeGroup(groupId);
    if(nodeGroup==NULL)
    {
        return -1;
    }
    else
    {
        ControlDate *controlDate=NULL;
        if(nodeGroup->getScheduleType()==NodeGroup::ScheduleByDate)
        {
            controlDate=getById(QString("%1_%2_%3").arg(groupId).arg(QDate::currentDate().month()).arg(QDate::currentDate().day()));
        }
        else if(nodeGroup->getScheduleType()==NodeGroup::ScheduleByOnDuty)
        {
            controlDate=getById(QString("%1_on_duty").arg(groupId));
        }
        else if(nodeGroup->getScheduleType()==NodeGroup::ScheduleByOffDuty)
        {
            controlDate=getById(QString("%1_off_duty").arg(groupId));
        }

        if(controlDate==NULL)
        {
            return -1;
        }
        else
        {
            return controlDate->controlPolicyId;
        }
    }

}

void ControlDatesModel::setControlDates(int groupId, QList<ControlDate *> *controlDateList)
{
    m_meshModel->dbManager()->dbCaller()->setControlDates(groupId, controlDateList);
}

void ControlDatesModel::doControlDatesChanged(QList<ControlDate*> *controlDateList)
{
    qDebug()<<"ControlDatesModel::doControlDatesChanged controlDateList.size="<<controlDateList->size();
    while(!m_controlDateList.isEmpty())
    {
        delete m_controlDateList.takeFirst();
    }
    m_controlDateMap.clear();
    int len=controlDateList->size();
    for(int i=0; i<len; i++)
    {
        ControlDate *controlDate=controlDateList->at(i);
        m_controlDateList.push_back(controlDate);
        m_controlDateMap.insert(controlDate->id, controlDate);
    }
    emit controlDateModelChanged();
}
