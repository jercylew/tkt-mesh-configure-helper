#ifndef TIMELINECONTROLITEMTABLEVIEWMODEL_H
#define TIMELINECONTROLITEMTABLEVIEWMODEL_H

#include <QObject>
#include <QStandardItemModel>
#include <QAbstractTableModel>
#include "timelinecontrolpolicymodel.h"

class MeshModel;

class TimeLineControlItemTableViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TimeLineControlItemTableViewModel(MeshModel *meshModel, quint8 groupId, int policySubId, QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void init();
    TimeLineControlPolicyModel *timeLineControlPolicyModel();

private:
    TimeLineControlPolicyModel *m_timeLineControlPolicyModel;

    quint8 m_groupId;
    int m_policySubId;

    MeshModel *m_meshModel;

public slots:
    void doDataChanged();
    void doModelChanged();
};

#endif // TIMELINECONTROLITEMTABLEVIEWMODEL_H
