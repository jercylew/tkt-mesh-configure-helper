#include "templatetimelinecontrolitemlistmodel.h"
#include "model/meshmodel.h"
#include "db/meshdbmanager.h"
#include "db/meshdbcaller.h"
#include <QDebug>

TemplateTimeLineControlItemListModel::TemplateTimeLineControlItemListModel(MeshModel *meshModel, const QString &policyId, QObject *parent) : QObject(parent)
{
    m_policyId=policyId;
    m_meshModel = meshModel;

    connect(m_meshModel->dbManager()->dbCaller(), SIGNAL(templateTimeLineControlItemListLoaded(QList<TimeLineControlItem*>*)),
            this, SLOT(doTemplateTimeLineControlItemListLoaded(QList<TimeLineControlItem*>*)));
    connect(m_meshModel->dbManager()->dbCaller(), SIGNAL(templateTimeLineControlItemLoaded(TimeLineControlItem*)),
            this, SLOT(doTemplateTimeLineControlItemLoaded(TimeLineControlItem*)));
}

void TemplateTimeLineControlItemListModel::init()
{
    refreshList();
}

int TemplateTimeLineControlItemListModel::count()
{
    return m_itemList.size();
}

TimeLineControlItem *TemplateTimeLineControlItemListModel::at(int index)
{
    if(m_itemList.size()==0)
    {
        return NULL;
    }
    else if(index<m_itemList.size())
    {
        return m_itemList.at(index);
    }
    else
    {
        return NULL;
    }
}

TimeLineControlItem *TemplateTimeLineControlItemListModel::getById(const QString &itemId)
{
    return m_itemMap.value(itemId);
}

QList<TimeLineControlItem *> TemplateTimeLineControlItemListModel::getAll()
{
    return this->m_itemList;
}

void TemplateTimeLineControlItemListModel::refreshList()
{
    m_meshModel->dbManager()->dbCaller()->loadTemplateTimeLineControlItemList(m_policyId);
}

void TemplateTimeLineControlItemListModel::doTemplateTimeLineControlItemListLoaded(QList<TimeLineControlItem *> *itemList)
{
    qDebug() << "TemplateTimeLineControlItemListModel::doTemplateTimeLineControlItemListLoaded size="
             << itemList->size();
    while(!m_itemList.isEmpty())
    {
        delete m_itemList.takeFirst();
    }
    m_itemMap.clear();

    for(int i = 0; i < itemList->size(); i++)
    {
        TimeLineControlItem *item = itemList->at(i);
        m_itemList.push_back(item);
        m_itemMap.insert(item->templateItemId, item);
    }
    emit modelChanged();
}

void TemplateTimeLineControlItemListModel::doTemplateTimeLineControlItemLoaded(TimeLineControlItem *itemNew)
{
    TimeLineControlItem *itemOld = m_itemMap.value(itemNew->templateItemId);
    if(itemOld != NULL)
    {
        itemOld->loadAll(itemNew);
        emit dataChanged();
    }
    delete itemNew;
}
