#include "timelinecontrolitemsmodel.h"
#include "db/meshdbmanager.h"
#include <QDebug>
#include "meshmodel.h"
#include "db/meshdbcaller.h"
#include "utils/timelinecontrolmanager.h"
#include "model/controldatesmodel.h"
#include "objectfactory.h"
#include "model/nodegroupsmodel.h"

bool TimeLineControlItemsModel__timeLineControlItemLessThan(TimeLineControlItem *item1, TimeLineControlItem *item2)
{
    return item1->startMinute < item2->startMinute;
}

TimeLineControlItemsModel::TimeLineControlItemsModel(MeshModel *meshModel, QObject *parent) : QObject(parent)
{
    this->m_meshModel=meshModel;
}

void TimeLineControlItemsModel::addControlItem(quint8 groupId, int policySubId, int startMinute)
{
    QList<TimeLineControlItem*> list=getTimeLineControlItemList(groupId, policySubId);
    NodeGroup *nodeGroup=m_meshModel->nodeGroupsModel()->getNodeGroup(groupId);
    if(nodeGroup==NULL)
    {
        return;
    }
    TimeLineControlItem *item=ObjectFactory::instance()->createTimeLineControlItemByGroupType(nodeGroup->getGroupType());
    item->groupId=groupId;
    item->policySubId=policySubId;
    item->startMinute=startMinute;
    if(!list.isEmpty())
    {
        int len=list.size();
        TimeLineControlItem *nearestItem=NULL;
        for(int i=0; i<len; i++)
        {
            if(nearestItem==NULL)
            {
                nearestItem=list.at(i);
            }
            else
            {
                int dMinuteNow=startMinute-list.at(i)->startMinute;
                int dMinuteOld=startMinute-nearestItem->startMinute;
                if(dMinuteNow>0 && dMinuteNow<dMinuteOld)
                {
                    nearestItem=list.at(i);
                }
            }
        }
        if(nearestItem!=NULL)
        {
            item->loadConfigData(nearestItem);
        }
    }

    m_meshModel->dbManager()->dbCaller()->addControlItem(item);
}

void TimeLineControlItemsModel::removeControlItem(int itemId)
{
    m_meshModel->dbManager()->dbCaller()->removeControlItem(itemId);
}

int TimeLineControlItemsModel::count(quint8 groupId, int policySubId)
{
    QList<TimeLineControlItem*> list=getTimeLineControlItemList(groupId, policySubId);
    return list.size();
}

QList<TimeLineControlItem *> TimeLineControlItemsModel::getTimeLineControlItemList(quint8 groupId, int policySubId)
{
    QList<TimeLineControlItem *> resultList;
    int len=m_itemList.size();
    qDebug()<<"=================>>> groupId="<<groupId<<", policySubId="<<policySubId;
    qDebug()<<"===========>size="<<m_itemList.size();
    for(int i=0; i<len; i++)
    {
        TimeLineControlItem *controlItem=m_itemList.at(i);
        if((controlItem->groupId==groupId) && (controlItem->policySubId==policySubId))
        {
            resultList.push_back(controlItem);
        }
    }

    qSort(resultList.begin(), resultList.end(), TimeLineControlItemsModel__timeLineControlItemLessThan);
    qDebug()<<"########## size="<<resultList.size();
    return resultList;
}

QList<TimeLineControlItem *> TimeLineControlItemsModel::getTodayTimeLineControlItemList(quint8 groupId)
{
    QList<TimeLineControlItem*> resultList;
    int todayPolicyId=m_meshModel->controlDatesModel()->getTodayPolicyId(groupId);
    if(todayPolicyId>=0)
    {
        TimeLineControlPolicy *controlPolicy=m_meshModel->timeLineControlManager()->timeLineControlPoliciesModel()->getById(todayPolicyId);
        if(controlPolicy!=NULL)
        {
            resultList=getTimeLineControlItemList(groupId, controlPolicy->subId);
        }
        else
        {
            resultList=getTimeLineControlItemList(groupId, 0);
        }
    }
    else
    {
        resultList=getTimeLineControlItemList(groupId, 0);
    }
    return resultList;
}

TimeLineControlItem *TimeLineControlItemsModel::getById(int id)
{
    return m_itemMap.value(id);
}

void TimeLineControlItemsModel::doControlItemsChanged(QList<TimeLineControlItem *> *itemList)
{
    qDebug()<<"TimeLineControlItemsModel::doControlItemsChanged itemList.size="<<itemList->size();

    while(!m_itemList.isEmpty())
    {
        delete m_itemList.takeFirst();
    }
    m_itemMap.clear();

    int size=itemList->size();
    for(int i=0; i<size; i++)
    {
        TimeLineControlItem *itemNew=itemList->at(i);
        m_itemList.push_back(itemNew);
        m_itemMap.insert(itemNew->id, itemNew);
    }
    delete itemList;
    qDebug()<<"TimeLineControlItemsModel::doControlItemsChanged !!";
    emit controlItemsModelChanged();
}

void TimeLineControlItemsModel::doControlItemChanged(TimeLineControlItem *itemNew)
{
    qDebug()<<"TimeLineControlItemsModel::doControlItemChanged item.startMinute="<<itemNew->startMinute<<", controlType="<<itemNew->controlType;
    TimeLineControlItem *itemOld=m_itemMap.value(itemNew->id);
    if(itemOld!=NULL)
    {
        itemOld->loadConfigData(itemNew);
        emit controlItemsDataChanged();
    }

    delete itemNew;
}
