#ifndef TIMELINECONTROLITEMLISTMODEL_H
#define TIMELINECONTROLITEMLISTMODEL_H

#include <QObject>
#include "domain/timelinecontrolitem.h"
#include "model/meshmodel.h"
#include <QList>

class TimeLineControlPolicyModel : public QObject
{
    Q_OBJECT
public:
    explicit TimeLineControlPolicyModel(MeshModel *meshModel, quint8 groupId, int controlPolicySubId, QObject *parent = 0);

    void init();

    int count();
    TimeLineControlItem *at(int index);

private:
    int m_controlPolicySubId;
    MeshModel *m_meshModel;
    quint8 m_groupId;
    QList<TimeLineControlItem *> m_timeLineControlItemList;

    void refreshList();

signals:
    void modelChanged();
    void dataChanged();

public slots:
    void doModelChanged();
    void doDataChanged();
};

#endif // TIMELINECONTROLITEMLISTMODEL_H
