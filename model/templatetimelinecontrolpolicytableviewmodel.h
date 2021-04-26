#ifndef TEMPLATETIMELINECONTROLITEMCOLLECTIONTABLEVIEWMODEL_H
#define TEMPLATETIMELINECONTROLITEMCOLLECTIONTABLEVIEWMODEL_H

#include <QAbstractTableModel>
#include "templatetimelinecontrolpolicylistmodel.h"

class MeshModel;
class NodeGroup;

class TemplateTimeLineControlPolicyTableViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TemplateTimeLineControlPolicyTableViewModel(MeshModel *meshModel, NodeGroup *group, QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    TemplateTimeLineControlPolicyListModel *templateTimeLineControlPolicyListModel();

private:
    TemplateTimeLineControlPolicyListModel *m_templateTimeLineControlPolicyListModel;

signals:

private slots:
    void doDataChanged();
    void doModelChanged();

private:
    MeshModel *m_meshModel;
    NodeGroup *m_group;
};

#endif // TEMPLATETIMELINECONTROLITEMCOLLECTIONTABLEVIEWMODEL_H
