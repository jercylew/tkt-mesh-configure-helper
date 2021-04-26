#ifndef TIMELINECONTROLITEMSMODEL_H
#define TIMELINECONTROLITEMSMODEL_H

#include <QObject>
#include "domain/timelinecontrolitem.h"
#include "domain/timelinecontrolpolicy.h"
#include <QMap>

class MeshModel;

class TimeLineControlItemsModel : public QObject
{
    Q_OBJECT
public:
    explicit TimeLineControlItemsModel(MeshModel *meshModel, QObject *parent = 0);

    void addControlItem(quint8 groupId, int policyId, int startMinute);

    void removeControlItem(int itemId);

    int count(quint8 groupId, int policyId);

    QList<TimeLineControlItem*> getTimeLineControlItemList(quint8 groupId, int policySubId);

    QList<TimeLineControlItem*> getTodayTimeLineControlItemList(quint8 groupId);

    TimeLineControlItem *getById(int id);

private:
    QMap<int, TimeLineControlItem*> m_itemMap;
    QList<TimeLineControlItem*> m_itemList;
    MeshModel *m_meshModel;

signals:
    void controlItemsModelChanged();
    void controlItemsDataChanged();

public slots:
    void doControlItemsChanged(QList<TimeLineControlItem*> *itemList);
    void doControlItemChanged(TimeLineControlItem *item);
};

#endif // TIMELINECONTROLITEMSMODEL_H
