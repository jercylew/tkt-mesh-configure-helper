#ifndef MESHDBCALLER_H
#define MESHDBCALLER_H

#include <QObject>
#include "meshdbholder.h"
#include "domain/nodegroup.h"
#include "domain/timelinecontrolitem.h"
#include "domain/timelinecontrolpolicy.h"
#include "domain/controldate.h"
#include "domain/templatetimelinecontrolpolicy.h"
#include "domain/timelinecontrolitem.h"
#include <QMutex>
#include <QMap>
#include <QPair>

class Sensor;
class Executer;

class MeshDbCaller : public QObject
{
    Q_OBJECT
public:
    explicit MeshDbCaller(const QString &meshUUID, QObject *parent = 0);
    ~MeshDbCaller();
    void addNodeGroup(NodeGroup *nodeGroup);
    void removeNodeGroup(quint8 groupId);
    void syncNodeGroups();
    void syncControlPolicies();
    void addControlPolicy(TimeLineControlPolicy *controlPolicy);
    void updateControlPolicy(TimeLineControlPolicy *controlPolicy);
    void removeControlPolicy(int policyId);
    void syncControlPolicy(int policyId);
    void addControlItem(TimeLineControlItem *controlItem);
    void syncControlItems();
    void updateControlItem(TimeLineControlItem *controlItem);
    void removeControlItem(int itemId);
    void syncControlItem(int itemId);
    void setExecuterNodesName(QMap<QString, QPair<QString, QString> > *nameMap);
    void setSensorNodesName(QMap<QString, QPair<QString, QString> > *nameMap);
    void addSensor(Sensor *sensor);
    void updateSensor(Sensor *sensor);
    void removeSensor(const QString &sensorId);
    void setNodeGroupsName(QMap<quint8, QString> *nameMap);
    void updateNodeGroupsScheduleData(NodeGroup *nodeGroup);
    void syncExecuterNodes();
    void syncSensorNodes();
    void setControlDates(quint8 groupId, QList<ControlDate*> *controlDateList);
    void syncControlDates();
    void deleteExecutersConfigData(QList<QString> *executerIdList);
    void deleteSensorsConfigData(QList<QString> *sensorIdList);
    void addControlItemsFromTemplate(NodeGroup *group, int subId, QList<TimeLineControlItem*> *templateItemList);
    void saveTemplateItemListAsNewCollection(TemplateTimeLineControlPolicy *policy, QList<TimeLineControlItem*> *itemList);
    void loadTemplateTimeLineControlPolicyList(NodeGroup *group = nullptr);
    void addTemplateTimeLineControlPolicy(TemplateTimeLineControlPolicy *policy);
    void removeTemplateTimeLineControlPolicy(QString policyId);
    void updateTemplateTimeLineControlPolicy(TemplateTimeLineControlPolicy *policy);
    void loadTemplateTimeLineControlPolicy(QString policyId);
    void loadTemplateTimeLineControlItemList(QString policyId);
    void addTemplateTimeLineControlItem(TimeLineControlItem *item);
    void updateTemplateTimeLineControlItem(TimeLineControlItem *item);
    void loadTemplateTimeLineControlItem(QString itemId);
    void removeTemplateTimeLineControlItem(QString policyId, QString itemId);

private:
    MeshDBHolder *dbReadOnlyHolder;
    MeshDBHolder *dbReadWriteHolder;
    QMutex *dbWriteMutex;

signals:

    void saveTemplateItemListAsNewCollectionFinished();
    void nodeGroupsChanged(QList<NodeGroup*> *nodeGroups);
    void controlPoliciesChanged(QList<TimeLineControlPolicy*> *itemListList);
    void controlPolicyChanged(TimeLineControlPolicy *itemList);
    void controlItemsChanged(QList<TimeLineControlItem*> *itemList);
    void controlItemChanged(TimeLineControlItem *item);
    void executerNodesChanged(QMap<QString, Executer*> *executerMap);
    void sensorNodesChanged(QMap<QString, Sensor*> *sensorMap);
    void controlDatesChanged(QList<ControlDate*> *controlDateList);
    void addControlItemsFromTemplateFinshed();
    void templateTimeLineControlPolicyListLoaded(QList<TemplateTimeLineControlPolicy*> *policyList);
    void templateTimeLineControlPolicyLoaded(TemplateTimeLineControlPolicy *policy);
    void templateTimeLineControlItemListLoaded(QList<TimeLineControlItem*> *itemList);
    void templateTimeLineControlItemLoaded(TimeLineControlItem *item);

public slots:
    void doAddNodeGroupTaskFinished();
    void doRemoveNodeGroupTaskFinished();
    void doSyncNodeGroupsTaskFinished();
    void doSyncControlPoliciesTaskFinished();
    void doAddControlPolicyTaskFinished();
    void doUpdateControlPolicyTaskFinished();
    void doRemoveControlPolicyTaskFinished();
    void doSyncControlPolicyTaskFinished();
    void doAddControlItemTaskFinished();
    void doSyncControlItemsTaskFinished();
    void doUpdateControlItemTaskFinished();
    void doRemoveControlItemTaskFinished();
    void doSyncControlItemTaskFinished();
    void doSetExecuterNodesNameTaskFinished();
    void doSetSensorNodesNameTaskFinished();
    void doUpdateNodeGroupsScheduleDataTaskFinished();
    void doAddSensorTaskFinished();
    void doUpdateSensorTaskFinished();
    void doRemoveSensorTaskFinished();
    void doSetNodeGroupsNameTaskFinished();
    void doSyncExecuterNodesTaskFinished();
    void doSyncSensorNodesTaskFinished();
    void doSetControlDatesTaskFinished();
    void doSyncControlDatesTaskFinished();
    void doAsyncDeleteExecutersConfigDataFinished();
    void doAsyncDeleteSensorsConfigDataFinished();
    void doSaveTemplateItemListAsNewCollectionTaskFinished();
    void doLoadTemplateTimeLineControlPolicyListTaskFinished();
    void doAddControlItemsFromTemplateTaskFinished();
    void doAddTemplateTimeLineControlPolicyTaskFinished();
    void doRemoveTemplateTimeLineControlPolicyTaskFinished();
    void doUpdateTemplateTimeLineControlPolicyTaskFinished();
    void doLoadTemplateTimeLineControlPolicyTaskFinished();
    void doLoadTemplateTimeLineControlItemListTaskFinished();
    void doAddTemplateTimeLineControlItemTaskFinished();
    void doUpdateTemplateTimeLineControlItemTaskFinished();
    void doLoadTemplateTimeLineControlItemTaskFinished();
    void doRemoveTemplateTimeLineControlItemTaskFinished();

};

#endif // MESHDBCALLER_H
