#include "templatetimelinecontrolpolicylistmodel.h"
#include <QDebug>
#include "model/meshmodel.h"
#include "db/meshdbmanager.h"
#include "db/meshdbcaller.h"

TemplateTimeLineControlPolicyListModel::TemplateTimeLineControlPolicyListModel(MeshModel *meshModel, NodeGroup *group, QObject *parent) : QObject(parent)
{
    m_meshModel = meshModel;
    m_group = group;
    connect(m_meshModel->dbManager()->dbCaller(), SIGNAL(templateTimeLineControlPolicyListLoaded(QList<TemplateTimeLineControlPolicy*>*)),
            this, SLOT(doTemplateTimeLineControlPolicyListLoaded(QList<TemplateTimeLineControlPolicy*>*)));
    connect(m_meshModel->dbManager()->dbCaller(), SIGNAL(templateTimeLineControlPolicyLoaded(TemplateTimeLineControlPolicy*)),
            this, SLOT(doTemplateTimeLineControlPolicyLoaded(TemplateTimeLineControlPolicy*)));
}

void TemplateTimeLineControlPolicyListModel::init()
{
    refreshList();
}

int TemplateTimeLineControlPolicyListModel::count()
{
    return m_itemCollectionList.size();
}

TemplateTimeLineControlPolicy *TemplateTimeLineControlPolicyListModel::at(int index)
{
    if(m_itemCollectionList.size()==0)
    {
        return NULL;
    }
    else if(index<m_itemCollectionList.size())
    {
        return m_itemCollectionList.at(index);
    }
    else
    {
        return NULL;
    }
}

TemplateTimeLineControlPolicy *TemplateTimeLineControlPolicyListModel::getById(const QString &collectionId)
{
    return m_itemCollectionMap.value(collectionId);
}

void TemplateTimeLineControlPolicyListModel::refreshList()
{
    m_meshModel->dbManager()->dbCaller()->loadTemplateTimeLineControlPolicyList(m_group);
}

void TemplateTimeLineControlPolicyListModel::doTemplateTimeLineControlPolicyListLoaded(QList<TemplateTimeLineControlPolicy *> *itemCollectionList)
{
    qDebug()<<"TemplateTimeLineControlPolicyListModel::doTemplateTimeLineControlPolicyListLoaded size="<<itemCollectionList->size();
    while(!m_itemCollectionList.isEmpty())
    {
        delete m_itemCollectionList.takeFirst();
    }
    m_itemCollectionMap.clear();

    int len=itemCollectionList->size();
    for(int i=0; i<len; i++)
    {
        TemplateTimeLineControlPolicy *itemCollection=itemCollectionList->at(i);

        if (itemCollection->groupType == NodeGroup::getTextByGroupType(m_group->getGroupType()))
        {
            m_itemCollectionList.push_back(itemCollection);
            m_itemCollectionMap.insert(itemCollection->id, itemCollection);
        }
    }
    emit modelChanged();
}

void TemplateTimeLineControlPolicyListModel::doTemplateTimeLineControlPolicyLoaded(TemplateTimeLineControlPolicy *itemCollection)
{
    TemplateTimeLineControlPolicy *oldItemCollection=m_itemCollectionMap.value(itemCollection->id);
    if(oldItemCollection!=NULL)
    {
        oldItemCollection->name=itemCollection->name;
        oldItemCollection->description=itemCollection->description;
        emit dataChanged();
    }
    delete itemCollection;
}
