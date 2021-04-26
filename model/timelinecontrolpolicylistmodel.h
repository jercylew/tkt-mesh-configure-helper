#ifndef TIMELINECONTROLITEMLISTLISTMODEL_H
#define TIMELINECONTROLITEMLISTLISTMODEL_H

#include <QObject>
#include <QList>
#include "timelinecontrolpolicymodel.h"

class TimeLineControlPolicyListModel : public QObject
{
    Q_OBJECT
public:
    explicit TimeLineControlPolicyListModel(MeshModel *meshModel, quint8 groupId, QObject *parent = 0);

    int count();

    void init();
    TimeLineControlPolicy *at(int index);

private:
    quint8 m_groupId;
    MeshModel *m_meshModel;
    QList<TimeLineControlPolicy *> m_timeLineControlPolicyList;

private slots:
    void refreshList();


signals:
    void dataChanged();
    void modelChanged();

public slots:
};

#endif // TIMELINECONTROLITEMLISTLISTMODEL_H
