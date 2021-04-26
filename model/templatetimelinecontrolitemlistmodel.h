#ifndef TEMPLATETIMELINECONTROLITEMLISTMODEL_H
#define TEMPLATETIMELINECONTROLITEMLISTMODEL_H

#include <QObject>
#include <QList>
#include <QMap>
#include "domain/timelinecontrolitem.h"

class MeshModel;

class TemplateTimeLineControlItemListModel : public QObject
{
    Q_OBJECT
public:
    explicit TemplateTimeLineControlItemListModel(MeshModel *meshModel, const QString &policyId, QObject *parent = 0);

    void init();
    int count();
    TimeLineControlItem *at(int index);
    TimeLineControlItem *getById(const QString &itemId);
    QList<TimeLineControlItem *> getAll();

private:
    QString m_policyId;
    QList<TimeLineControlItem*> m_itemList;
    QMap<QString, TimeLineControlItem*> m_itemMap;
    void refreshList();

signals:
    void modelChanged();
    void dataChanged();

private slots:
    void doTemplateTimeLineControlItemListLoaded(QList<TimeLineControlItem*>*);
    void doTemplateTimeLineControlItemLoaded(TimeLineControlItem*);

private:
    MeshModel *m_meshModel;
};

#endif // TEMPLATETIMELINECONTROLITEMLISTMODEL_H
