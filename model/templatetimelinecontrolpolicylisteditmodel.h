#ifndef TEMPLATETIMELINECONTROLITEMCOLLECTIONLISTEDITMODEL_H
#define TEMPLATETIMELINECONTROLITEMCOLLECTIONLISTEDITMODEL_H

#include <QObject>
#include "templatetimelinecontrolpolicytableviewmodel.h"

class MeshModel;
class NodeGroup;

class TemplateTimeLineControlPolicyListEditModel : public QObject
{
    Q_OBJECT
public:
    explicit TemplateTimeLineControlPolicyListEditModel(MeshModel *meshModel, NodeGroup *group, QObject *parent = 0);

    TemplateTimeLineControlPolicyTableViewModel *templateTimeLineControlPolicyTableViewModel();
    void addTemplateTimeLineControlPolicy(const QString &name, const QString &description);
    void removeTemplateTimeLineControlPolicy(const QString &itemCollectionId);
    void updateTemplateTimeLineControlPolicy(TemplateTimeLineControlPolicy *itemCollection);

private:
    TemplateTimeLineControlPolicyTableViewModel *m_templateTimeLineControlPolicyTableViewModel;

signals:

public slots:

private:
    MeshModel *m_meshModel;
    NodeGroup *m_group;
};

#endif // TEMPLATETIMELINECONTROLITEMCOLLECTIONLISTEDITMODEL_H
