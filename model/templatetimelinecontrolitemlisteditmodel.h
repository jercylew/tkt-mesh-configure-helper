#ifndef TEMPLATETIMELINECONTROLITEMLISTEDITMODEL_H
#define TEMPLATETIMELINECONTROLITEMLISTEDITMODEL_H

#include <QObject>
#include "templatetimelinecontrolitemtableviewmodel.h"

class MeshModel;

class TemplateTimeLineControlItemListEditModel : public QObject
{
    Q_OBJECT
public:
    explicit TemplateTimeLineControlItemListEditModel(MeshModel *meshModel, NodeGroup *group, const QString &policyId, QObject *parent = 0);

    TemplateTimeLineControlItemTableViewModel *templateTimeLineControlItemTableViewModel();

    void addTemplateControlItem(const QString &policyId, int startMinute);
    void updateTemplateControlItem(TimeLineControlItem *templateItem);
    void removeTemplateControlItem(const QString &policyId, const QString &itemId);

private:
    TemplateTimeLineControlItemTableViewModel *m_templateTimeLineControlItemTableViewModel;

signals:

public slots:

private:
    MeshModel *m_meshModel;
    NodeGroup *m_group;
};

#endif // TEMPLATETIMELINECONTROLITEMLISTEDITMODEL_H
