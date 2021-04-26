#include "timelinecontrolmanager.h"
#include "db/meshdbmanager.h"
#include "model/sensorsmodel.h"
#include "db/meshdbcaller.h"

TimeLineControlPoliciesModel *TimeLineControlManager::timeLineControlPoliciesModel()
{
    return this->m_timeLineControlPoliciesModel;
}

TimeLineControlItemsModel *TimeLineControlManager::getTimeLineControlItemsModel()
{
    return this->timeLineControlItemsModel;
}

void TimeLineControlManager::init()
{

}

TimeLineControlManager::TimeLineControlManager(MeshModel *meshModel, MeshDBManager *dbManager, SensorsModel *sensorsModel, QObject *parent)
    : QObject(parent),
      m_dbManager(dbManager),
      m_sensorsModel(sensorsModel),
      m_meshModel(meshModel)
{
    m_timeLineControlPoliciesModel=new TimeLineControlPoliciesModel(m_meshModel);
    timeLineControlItemsModel=new TimeLineControlItemsModel(m_meshModel);

    connect(m_dbManager->dbCaller(), SIGNAL(controlPoliciesChanged(QList<TimeLineControlPolicy*>*)),
            m_timeLineControlPoliciesModel, SLOT(doControlPoliciesChanged(QList<TimeLineControlPolicy*>*)));
    connect(m_dbManager->dbCaller(), SIGNAL(controlPolicyChanged(TimeLineControlPolicy*)),
            m_timeLineControlPoliciesModel, SLOT(doControlPolicyChanged(TimeLineControlPolicy*)));

    connect(m_dbManager->dbCaller(), SIGNAL(controlItemsChanged(QList<TimeLineControlItem*>*)),
            timeLineControlItemsModel, SLOT(doControlItemsChanged(QList<TimeLineControlItem*>*)));
    connect(m_dbManager->dbCaller(), SIGNAL(controlItemChanged(TimeLineControlItem*)),
            timeLineControlItemsModel, SLOT(doControlItemChanged(TimeLineControlItem*)));
}
