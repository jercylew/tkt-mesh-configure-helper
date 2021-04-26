#include "templatetimelinecontrolpolicylisteditmodel.h"
#include "model/meshmodel.h"
#include "db/meshdbmanager.h"
#include "db/meshdbcaller.h"
#include <QUuid>

TemplateTimeLineControlPolicyListEditModel::TemplateTimeLineControlPolicyListEditModel(MeshModel *meshModel, NodeGroup *group, QObject *parent) : QObject(parent)
{
    m_meshModel = meshModel;
    m_group = group;
    m_templateTimeLineControlPolicyTableViewModel=new TemplateTimeLineControlPolicyTableViewModel(m_meshModel, m_group, this);
}

TemplateTimeLineControlPolicyTableViewModel *TemplateTimeLineControlPolicyListEditModel::templateTimeLineControlPolicyTableViewModel()
{
    return m_templateTimeLineControlPolicyTableViewModel;
}

void TemplateTimeLineControlPolicyListEditModel::addTemplateTimeLineControlPolicy(const QString &name, const QString &description)
{
    TemplateTimeLineControlPolicy *itemCollection=new TemplateTimeLineControlPolicy;
    itemCollection->id=QUuid::createUuid().toString().remove("{").remove("}").remove("-");
    itemCollection->name=name;
    itemCollection->description=description;
    itemCollection->groupType = NodeGroup::getTextByGroupType(m_group->getGroupType());

    m_meshModel->dbManager()->dbCaller()->addTemplateTimeLineControlPolicy(itemCollection);
}

void TemplateTimeLineControlPolicyListEditModel::removeTemplateTimeLineControlPolicy(const QString &itemCollectionId)
{
    m_meshModel->dbManager()->dbCaller()->removeTemplateTimeLineControlPolicy(itemCollectionId);
}

void TemplateTimeLineControlPolicyListEditModel::updateTemplateTimeLineControlPolicy(TemplateTimeLineControlPolicy *itemCollection)
{
    m_meshModel->dbManager()->dbCaller()->updateTemplateTimeLineControlPolicy(itemCollection);
}
