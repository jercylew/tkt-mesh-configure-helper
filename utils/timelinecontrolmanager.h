#ifndef TIMELINECONTROLMANAGER_H
#define TIMELINECONTROLMANAGER_H

#include <QObject>
#include "model/timelinecontrolitemtableviewmodel.h"
#include "model/timelinecontrolitemsmodel.h"
#include "model/timelinecontrolpolicymodel.h"
#include "model/timelinecontrolpoliciesmodel.h"

class MeshDBManager;
class SensorsModel;
class MeshModel;

class TimeLineControlManager : public QObject
{
    Q_OBJECT
public:
    explicit TimeLineControlManager(MeshModel *meshModel, MeshDBManager *dbManager, SensorsModel *sensorsModel, QObject *parent = 0);

    TimeLineControlPoliciesModel *timeLineControlPoliciesModel();
    TimeLineControlItemsModel *getTimeLineControlItemsModel();
    void init();

private:
    TimeLineControlPoliciesModel *m_timeLineControlPoliciesModel;
    TimeLineControlItemsModel *timeLineControlItemsModel;

    MeshDBManager *m_dbManager;
    SensorsModel *m_sensorsModel;
    MeshModel *m_meshModel;

signals:

public slots:
};

#endif // TIMELINECONTROLMANAGER_H
