#include "templatetimelinecontrolitemlisteditmodel.h"
#include "model/meshmodel.h"
#include "db/meshdbmanager.h"
#include "db/meshdbcaller.h"
#include "utils/tools.h"
#include "objectfactory.h"
#include <QUuid>
#include <QDebug>

TemplateTimeLineControlItemListEditModel::TemplateTimeLineControlItemListEditModel(MeshModel *meshModel, NodeGroup *group,
                                                                                   const QString &policyId, QObject *parent) : QObject(parent)
{
    m_meshModel = meshModel;
    m_group = group;
    m_templateTimeLineControlItemTableViewModel = new TemplateTimeLineControlItemTableViewModel(meshModel, policyId, this);
}

TemplateTimeLineControlItemTableViewModel *TemplateTimeLineControlItemListEditModel::templateTimeLineControlItemTableViewModel()
{
    return m_templateTimeLineControlItemTableViewModel;
}

void TemplateTimeLineControlItemListEditModel::addTemplateControlItem(const QString &policyId, int startMinute)
{
    QList<TimeLineControlItem*> list = m_templateTimeLineControlItemTableViewModel->templateTimeLineControlItemListModel()->getAll();

    TimeLineControlItem *item = ObjectFactory::instance()->createTimeLineControlItemByGroupType(m_group->getGroupType());

    if(!list.isEmpty())
    {
        int len=list.size();
        TimeLineControlItem *nearestItem = nullptr;
        for(int i=0; i<len; i++)
        {
            if(nearestItem == nullptr)
            {
                nearestItem=list.at(i);
            }
            else
            {
                int dMinuteNow=startMinute-list.at(i)->startMinute;
                int dMinuteOld=startMinute-nearestItem->startMinute;
                if(dMinuteNow>0 && dMinuteNow<dMinuteOld)
                {
                    nearestItem = list.at(i);
                }
            }
        }
        if(nearestItem != nullptr)
        {
            item->loadAll(nearestItem);
        }
    }

    item->templateItemId = Tools::getRandomUUID();
    item->templatePolicyId = policyId;
    item->startMinute = startMinute;

    m_meshModel->dbManager()->dbCaller()->addTemplateTimeLineControlItem(item);
}

void TemplateTimeLineControlItemListEditModel::updateTemplateControlItem(TimeLineControlItem *templateItem)
{
    m_meshModel->dbManager()->dbCaller()->updateTemplateTimeLineControlItem(templateItem);
}

void TemplateTimeLineControlItemListEditModel::removeTemplateControlItem(const QString &policyId, const QString &itemId)
{
    m_meshModel->dbManager()->dbCaller()->removeTemplateTimeLineControlItem(policyId, itemId);
}
