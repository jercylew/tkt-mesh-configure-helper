#ifndef TEMPLATETIMELINECONTROLITEMCOLLECTIONLISTMODEL_H
#define TEMPLATETIMELINECONTROLITEMCOLLECTIONLISTMODEL_H

#include <QObject>
#include <QList>
#include <QMap>
#include "domain/templatetimelinecontrolpolicy.h"

class MeshModel;
class NodeGroup;

class TemplateTimeLineControlPolicyListModel : public QObject
{
    Q_OBJECT
public:
    explicit TemplateTimeLineControlPolicyListModel(MeshModel *meshModel, NodeGroup *group, QObject *parent = 0);

    void init();
    int count();
    TemplateTimeLineControlPolicy *at(int index);
    TemplateTimeLineControlPolicy *getById(const QString &collectionId);

private:
    QList<TemplateTimeLineControlPolicy*> m_itemCollectionList;
    QMap<QString, TemplateTimeLineControlPolicy*> m_itemCollectionMap;
    void refreshList();

signals:
    void dataChanged();
    void modelChanged();

private slots:
    void doTemplateTimeLineControlPolicyListLoaded(QList<TemplateTimeLineControlPolicy*>*);
    void doTemplateTimeLineControlPolicyLoaded(TemplateTimeLineControlPolicy*);

private:
    MeshModel *m_meshModel;
    NodeGroup *m_group;
};

#endif // TEMPLATETIMELINECONTROLITEMCOLLECTIONLISTMODEL_H
