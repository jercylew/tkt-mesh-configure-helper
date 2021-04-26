#ifndef TIMELINECONTROLITEMLISTVIEWMODEL_H
#define TIMELINECONTROLITEMLISTVIEWMODEL_H

#include <QAbstractTableModel>
#include "model/meshmodel.h"
#include "model/timelinecontrolpolicylistmodel.h"

class TimeLineControlPolicyTableViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TimeLineControlPolicyTableViewModel(MeshModel *meshModel, quint8 groupId, QObject *parent=NULL);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    TimeLineControlPolicyListModel *timeLineControlPolicyListModel();

private:
    MeshModel *m_meshModel;
    quint8 m_groupId;
    TimeLineControlPolicyListModel *m_timeLineControlPolicyListModel;

private slots:
    void doModelChanged();
    void doDataChanged();
};

#endif // TIMELINECONTROLITEMLISTVIEWMODEL_H
