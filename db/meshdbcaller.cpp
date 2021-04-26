#include "meshdbcaller.h"
#include <QFuture>
#include <QtConcurrent>
#include <QMutexLocker>
#include <QSqlQuery>
#include <QSqlError>
#include "domain/nodegroup.h"
#include "domain/timelinecontrolitem.h"
#include "domain/luxsensor.h"
#include "domain/realmotionsensor.h"
#include "domain/virtualmotionsensor.h"
#include "domain/pmsensor.h"
#include "domain/co1sensor.h"
#include "domain/co2sensor.h"
#include "domain/virtualswitchsensor.h"
#include "domain/realmotionandluxcamerasensor.h"
#include "domain/realswitchsensor.h"
#include "domain/templatetimelinecontrolpolicy.h"
#include "parsermanager.h"
#include "objectfactory.h"
#include "parser/sensorparser.h"
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include "utils/tools.h"

#include "parser/executerparser.h"

bool timeLineControlItemLessThan(TimeLineControlItem *item1, TimeLineControlItem *item2)
{
    return item1->startMinute < item2->startMinute;
}

bool timeLineControlPolicyLessThan(TimeLineControlPolicy *policy1, TimeLineControlPolicy *policy2)
{
    return policy1->id < policy2->id;
}

void asyncAddTemplateTimeLineControlPolicy(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, TemplateTimeLineControlPolicy *policy)
{
    QMutexLocker locker(dbWriteMutex);
    QSqlQuery addQuery(dbWriteHolder->getDb());
    addQuery.exec(QString("insert into template_control_policy(policy_id, policy_name, group_type, create_msec)values('%1', '%2', '%3', %4)")
                .arg(policy->id)
                .arg(policy->name)
                .arg(policy->groupType)
                .arg(QDateTime::currentMSecsSinceEpoch()));
}

void asyncRemoveTemplateTimeLineControlPolicy(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, QString policyId)
{
    QMutexLocker locker(dbWriteMutex);
    dbWriteHolder->getDb().transaction();
    QSqlQuery deletePolicyQuery(dbWriteHolder->getDb());
    deletePolicyQuery.exec(QString("delete from template_control_policy where policy_id='%1'").arg(policyId));
    QSqlQuery deletePolicyPropertyQuery(dbWriteHolder->getDb());
    deletePolicyPropertyQuery.exec(QString("delete from template_control_policy_property where policy_id='%1'").arg(policyId));
    QSqlQuery deleteControlItemPropertyQuery(dbWriteHolder->getDb());
    deleteControlItemPropertyQuery.exec(QString("delete from template_control_item_property where item_id in (select item_id from template_control_item where policy_id='%1')").arg(policyId));
    QSqlQuery deleteControlItemQuery(dbWriteHolder->getDb());
    deleteControlItemQuery.exec(QString("delete from template_control_item where policy_id='%1'").arg(policyId));
    dbWriteHolder->getDb().commit();
}

void asyncAddNodeGroup(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, NodeGroup *nodeGroup)
{
    QMutexLocker locker(dbWriteMutex);
    dbWriteHolder->getDb().transaction();
    QSqlQuery addNodeGroupQuery(dbWriteHolder->getDb());
    addNodeGroupQuery.exec(QString("insert into node_group(id, name, color, group_data) values(%1, '%2', '%3', '%4')")
                                                .arg(nodeGroup->getId()&0xFF)
                                                .arg(nodeGroup->getName())
                                                .arg(nodeGroup->getColor())
                                                .arg(nodeGroup->buildGroupData()));
    QSqlQuery addDefaultPolicyQuery(dbWriteHolder->getDb());
    addDefaultPolicyQuery.exec(QString("replace into control_policy(id, group_id, name, color, sub_id) values (%1, %2, '%3', '%4', %5)")
                               .arg(nodeGroup->getId()*100+0)
                               .arg(nodeGroup->getId()&0xFF)
                               .arg("Default Policy")
                               .arg("#FFFFFF")
                               .arg(0));
    dbWriteHolder->getDb().commit();
    delete nodeGroup;
}

void asyncRemoveNodeGroup(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, quint8 groupId)
{
    QMutexLocker locker(dbWriteMutex);
    dbWriteHolder->getDb().transaction();
    QSqlQuery query(dbWriteHolder->getDb());
    query.exec(QString("delete from node_group where id=%1").arg(groupId&0xFF));
    query.exec(QString("delete from control_policy where group_id=%1").arg(groupId&0xFF));
    query.exec(QString("delete from control_item where group_id=%1").arg(groupId&0xFF));
    dbWriteHolder->getDb().commit();
}

QList<NodeGroup*> *asyncGetNodeGroups(MeshDBHolder *dbReadHolder)
{
    QSqlQuery groupQuery(dbReadHolder->getDb());
    groupQuery.exec(QString("select * from node_group"));
    QList<NodeGroup*> *nodeGroups=new QList<NodeGroup*>;
    while(groupQuery.next())
    {
        NodeGroup *group=new NodeGroup;
        group->setId((quint8)(groupQuery.value("id").toInt()));
        group->setName(groupQuery.value("name").toString());
        group->setColor(groupQuery.value("color").toString());
        group->loadParamsValueByGroupData(groupQuery.value("group_data").toString());
        nodeGroups->push_back(group);
    }
    return nodeGroups;
}

QList<TimeLineControlPolicy*> *asyncGetControlPolicies(MeshDBHolder *dbReadHolder)
{
    QSqlQuery query(dbReadHolder->getDb());
    query.exec(QString("select * from control_policy"));
    QList<TimeLineControlPolicy*> *policyList=new QList<TimeLineControlPolicy*>;
    while(query.next())
    {
        TimeLineControlPolicy *controlPolicy=new TimeLineControlPolicy;
        controlPolicy->id=query.value("id").toInt();
        controlPolicy->groupId=(quint8)(query.value("group_id").toInt());
        controlPolicy->name=query.value("name").toString();
        controlPolicy->color=query.value("color").toString();
        controlPolicy->subId=query.value("sub_id").toInt();
        policyList->push_back(controlPolicy);
    }
    return policyList;
}

void asyncUpdateControlPolicy(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, TimeLineControlPolicy *controlPolicy)
{
    QMutexLocker locker(dbWriteMutex);
    QString cmd=QString("update control_policy set name='%1', color='%2' where id=%3")
            .arg(controlPolicy->name)
            .arg(controlPolicy->color)
            .arg(controlPolicy->id);
    QSqlQuery query=dbWriteHolder->getDb().exec(cmd);   
}

void asyncRemoveControlPolicy(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, int policyId)
{
    if((policyId%100)==0)
    {
        return;  //不删除默认策略
    }
    QMutexLocker locker(dbWriteMutex);
    dbWriteHolder->getDb().transaction();
    QSqlQuery removeControlPolicyQuery(dbWriteHolder->getDb());
    removeControlPolicyQuery.exec(QString("delete from control_policy where id=%1").arg(policyId));
    QSqlQuery removeControlItemQuery(dbWriteHolder->getDb());
    int groupId=(policyId/100);
    int subId=(policyId%100);
    removeControlItemQuery.exec(QString("delete from control_item where group_id=%1 and policy_sub_id=%2")
                                .arg(groupId)
                                .arg(subId));
    dbWriteHolder->getDb().commit();
}

void asyncAddControlPolicy(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, TimeLineControlPolicy *controlPolicy)
{
    QMutexLocker locker(dbWriteMutex);
    QString cmd=QString("insert into control_policy(id, group_id, name, color, sub_id)values(%1, %2, '%3', '%4', %5)")
            .arg(controlPolicy->id)
            .arg(controlPolicy->groupId&0xFF)
            .arg(controlPolicy->name)
            .arg(controlPolicy->color)
            .arg(controlPolicy->subId);
    QSqlQuery query(dbWriteHolder->getDb());
    query.exec(cmd);
}
TimeLineControlPolicy *asyncGetControlPolicy(MeshDBHolder *dbReadHolder, int id)
{
    QSqlQuery query=dbReadHolder->getDb().exec(QString("select * from control_policy where id=%1").arg(id));
    TimeLineControlPolicy *controlPolicy=new TimeLineControlPolicy;
    if(query.next())
    {
        controlPolicy->id=query.value("id").toInt();
        controlPolicy->groupId=(quint8)(query.value("group_id").toInt());
        controlPolicy->name=query.value("name").toString();
        controlPolicy->color=query.value("color").toString();
        controlPolicy->subId=query.value("sub_id").toInt();
    }
    return controlPolicy;
}

void asyncAddControlItem(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, TimeLineControlItem *controlItem)
{
    QMutexLocker locker(dbWriteMutex);

    QJsonObject *controlDataJson = new QJsonObject;
    controlItem->controlDataToJson(controlDataJson);
    QString controlDataText=QString::fromUtf8(Tools::getDataArrayOfJsonObject(controlDataJson));

    QString cmd=QString("insert into control_item("
                        "id, "
                        "group_id, "
                        "policy_sub_id, "
                        "control_data) values(%1, %2, %3, '%4')")
            .arg((controlItem->groupId&0xFF)*10000*100+((int)controlItem->policySubId)*10000+controlItem->startMinute)
            .arg((quint16)controlItem->groupId)
            .arg(controlItem->policySubId)
            .arg(controlDataText);
    QSqlQuery query=dbWriteHolder->getDb().exec(cmd);
    qDebug()<<"asyncAddControlItem, error="<<query.lastError().text();
    delete controlItem;
    delete controlDataJson;
}

QList<TimeLineControlItem*> *asyncGetControlItems(MeshDBHolder *dbReadHolder)
{
    QSqlQuery groupTypeQuery(dbReadHolder->getDb());
    groupTypeQuery.exec(QString("select * from node_group"));
    QMap<quint8, QString> *groupTypeMap=new QMap<quint8, QString>;
    while(groupTypeQuery.next())
    {
        QJsonObject groupDataJson=Tools::getJsonObjectFromDataArray(groupTypeQuery.value("group_data").toString().toUtf8());
        if(groupDataJson.isEmpty())
        {
            continue;
        }
        groupTypeMap->insert(groupTypeQuery.value("id").toInt(), groupDataJson.value("group_type").toString());
    }
    QSqlQuery controlItemQuery=dbReadHolder->getDb().exec(QString("select * from control_item"));
    qDebug()<<"asyncGetControlItems, error="<<controlItemQuery.lastError().text();
    QList<TimeLineControlItem*> *itemGroups=new QList<TimeLineControlItem*>;
    while(controlItemQuery.next())
    {
        quint8 groupId = (quint8)(controlItemQuery.value("group_id").toInt());
        TimeLineControlItem *item = ObjectFactory::instance()->createTimeLineControlItemByGroupTypeText(groupTypeMap->value(groupId));
        item->id = controlItemQuery.value("id").toInt();
        item->groupId = groupId;
        item->policySubId = controlItemQuery.value("policy_sub_id").toInt();

        QByteArray controDataText = controlItemQuery.value("control_data").toString().toUtf8();
        QJsonObject controlDataJson = Tools::getJsonObjectFromDataArray(controDataText);
        item->controlDataFromJson(&controlDataJson);
        itemGroups->push_back(item);
    }
    std::sort(itemGroups->begin(), itemGroups->end(), timeLineControlItemLessThan);
    return itemGroups;
}

TimeLineControlItem *asyncGetControlItem(MeshDBHolder *dbReadHolder, int id)
{
    QSqlQuery controlItemQuery=dbReadHolder->getDb().exec(QString("select * from control_item where id=%1").arg(id));
    qDebug()<<"asyncGetControlItem, error="<<controlItemQuery.lastError().text();
    TimeLineControlItem *item=NULL;
    if(controlItemQuery.next())
    {
        quint8 groupId=controlItemQuery.value("group_id").toInt();
        QString groupType="";
        QSqlQuery groupTypeQuery(dbReadHolder->getDb());
        groupTypeQuery.exec(QString("select * from node_group where id=%1").arg(groupId&0xFF));
        if(groupTypeQuery.next())
        {
            QJsonObject groupDataJson=Tools::getJsonObjectFromDataArray(groupTypeQuery.value("group_data").toString().toUtf8());
            if(!groupDataJson.isEmpty())
            {
                groupType=groupDataJson.value("group_type").toString();
            }
        }

        item=ObjectFactory::instance()->createTimeLineControlItemByGroupTypeText(groupType);
        item->id=controlItemQuery.value("id").toInt();
        item->groupId=groupId;
        item->policySubId=controlItemQuery.value("policy_sub_id").toInt();

        QJsonObject controlDataJson=Tools::getJsonObjectFromDataArray(controlItemQuery.value("control_data").toString().toUtf8());
        item->controlDataFromJson(&controlDataJson);
    }
    return item;
}

void asyncUpdateControlItem(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, TimeLineControlItem *controlItem)
{
    QMutexLocker locker(dbWriteMutex);

    QJsonObject *controlDataJson=new QJsonObject;
    controlItem->controlDataToJson(controlDataJson);
    QString controlDataText=QString::fromUtf8(Tools::getDataArrayOfJsonObject(controlDataJson));

    QString cmd = QString("update control_item set control_data='%1' where id=%2")
            .arg(controlDataText)
            .arg(controlItem->id);
    QSqlQuery query=dbWriteHolder->getDb().exec(cmd);
    qDebug()<<"asyncUpdateControlItem, error="<<query.lastError().text();
    delete controlItem;
    delete controlDataJson;
}

void asyncRemoveControlItem(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, int itemId)
{
    QMutexLocker locker(dbWriteMutex);
    QString cmd=QString("delete from control_item where id=%1").arg(itemId);
    QSqlQuery query=dbWriteHolder->getDb().exec(cmd);
    qDebug()<<"asyncRemoveControlItem, error="<<query.lastError().text();
}

void asyncSetExecuterNodesName(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, QMap<QString, QPair<QString, QString> > *nameMap)
{
    QMutexLocker locker(dbWriteMutex);
    dbWriteHolder->getDb().transaction();
    QList<QString> keys=nameMap->keys();
    foreach (QString id, keys) {
        QPair<QString, QString> namePair=nameMap->value(id);
        QSqlQuery updateExecuterQuery(dbWriteHolder->getDb());
        updateExecuterQuery.exec(QString("replace into executer_node(executer_id, executer_type, executer_name) values('%1', '%2', '%3')").arg(id).arg(namePair.first).arg(namePair.second));
    }
    dbWriteHolder->getDb().commit();
    delete nameMap;
}

void asyncSetSensorNodesName(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, QMap<QString, QPair<QString, QString> > *nameMap)
{
    QMutexLocker locker(dbWriteMutex);
    dbWriteHolder->getDb().transaction();
    QList<QString> keys=nameMap->keys();
    foreach (QString id, keys) {
        QPair<QString, QString> namePair=nameMap->value(id);
        QSqlQuery updateSensorQuery(dbWriteHolder->getDb());
        updateSensorQuery.exec(QString("replace into sensor_node(sensor_id, sensor_type, sensor_name) values('%1', '%2', '%3')").arg(id).arg(namePair.first).arg(namePair.second));
    }
    dbWriteHolder->getDb().commit();
    delete nameMap;
}

void asyncAddSensor(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, Sensor *sensor)
{
    QString sensorData;
    SensorParser *sensorParser=ParserManager::instance()->getSensorParserByTypeText(sensor->typeText());
    if(sensorParser!=NULL)
    {
        sensorData=sensorParser->buildSensorDataFromSensor(sensor);
    }
    QMutexLocker locker(dbWriteMutex);
    dbWriteHolder->getDb().transaction();
    QSqlQuery addSensorQuery(dbWriteHolder->getDb());
    addSensorQuery.exec(QString("replace into sensor_node(sensor_id, sensor_type, sensor_name, sensor_data) values('%1', '%2', '%3', '%4')")
            .arg(sensor->sensorId())
            .arg(sensor->typeText())
            .arg(sensor->getName())
            .arg(sensorData));
    dbWriteHolder->getDb().commit();
    delete sensor;
}

void asyncUpdateSensor(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, Sensor *sensor)
{
    QString sensorData;
    SensorParser *sensorParser=ParserManager::instance()->getSensorParserByTypeText(sensor->typeText());
    if(sensorParser!=NULL)
    {
        sensorData=sensorParser->buildSensorDataFromSensor(sensor);
    }
    QMutexLocker locker(dbWriteMutex);
    dbWriteHolder->getDb().transaction();
    QSqlQuery updateSensorQuery(dbWriteHolder->getDb());
    updateSensorQuery.exec(QString("replace into sensor_node(sensor_id, sensor_type, sensor_name, sensor_data)values('%1', '%2', '%3', '%4')")
                                  .arg(sensor->sensorId())
                                  .arg(sensor->typeText())
                                  .arg(sensor->getName())
                                  .arg(sensorData));
    dbWriteHolder->getDb().commit();
    delete sensor;
}

void asyncRemoveSensor(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, const QString &sensorId)
{
    QMutexLocker locker(dbWriteMutex);
    QSqlQuery removeSensorQuery(dbWriteHolder->getDb());
    removeSensorQuery.exec(QString("delete from sensor_node where sensor_id='%1'").arg(sensorId));
}

void asyncSetNodeGroupsName(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, QMap<quint8, QString> *nameMap)
{
    QMutexLocker locker(dbWriteMutex);
    dbWriteHolder->getDb().transaction();
    QList<quint8> keys=nameMap->keys();
    foreach (quint8 id, keys) {
        QSqlQuery updateNodeGroupQuery(dbWriteHolder->getDb());
        updateNodeGroupQuery.exec(QString("update node_group set name='%1' where id=%2")
                                    .arg(nameMap->value(id))
                                    .arg(id&0xFF));
    }
    dbWriteHolder->getDb().commit();
    delete nameMap;
}

void asyncUpdateNodeGroupScheduleData(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, NodeGroup *nodeGroup)
{
    QMutexLocker locker(dbWriteMutex);
    QSqlQuery query=dbWriteHolder->getDb().exec(QString("update node_group set group_data='%1' where id=%2")
                                                .arg(nodeGroup->buildGroupData())
                                                .arg(nodeGroup->getId()));
    qDebug()<<"asyncUpdateNodeGroupScheduleData, error="<<query.lastError().text();
    delete nodeGroup;
}

QMap<QString, Executer*> *asyncGetExecuterNodes(MeshDBHolder *dbReadHolder)
{
    QSqlQuery getExecuterNodesQuery(dbReadHolder->getDb());
    getExecuterNodesQuery.exec(QString("select * from executer_node"));
    QMap<QString, Executer*> *executerMap=new QMap<QString, Executer*>;
    while(getExecuterNodesQuery.next())
    {
        QString executerTypeText=getExecuterNodesQuery.value("executer_type").toString();
        Executer *executer=ObjectFactory::instance()->createExecuterByTypeText(executerTypeText);
        if(executer==NULL)
        {
            continue;
        }
        ExecuterParser *executerParser=ParserManager::instance()->getExecuterParserByTypeText(executerTypeText);
        if(executerParser==NULL)
        {
            continue;
        }
        executerParser->parseDBDataToExecuter(&getExecuterNodesQuery, executer);
        executerMap->insert(executer->executerId(), executer);
    }

    return executerMap;
}

QMap<QString, QString> *asyncGetExecutersName(MeshDBHolder *dbReadHolder)
{
    QSqlQuery query=dbReadHolder->getDb().exec(QString("select * from executer_node"));
    qDebug()<<"asyncGetExecutersName, error="<<query.lastError().text();
    QMap<QString, QString> *nameMap=new QMap<QString, QString>;
    while(query.next())
    {
        nameMap->insert(query.value("executer_id").toString(), query.value("executer_name").toString());
    }
    return nameMap;
}

QMap<QString, Sensor*> *asyncGetSensorNodes(MeshDBHolder *dbReadHolder)
{
    QSqlQuery query=dbReadHolder->getDb().exec(QString("select * from sensor_node"));
    qDebug()<<"asyncGetSensorNodes, error="<<query.lastError().text();
    QMap<QString, Sensor*> *sensorMap=new QMap<QString, Sensor*>;
    while(query.next())
    {
        QString sensorTypeText=query.value("sensor_type").toString();
        Sensor *sensor=ObjectFactory::instance()->createSensorByTypeText(sensorTypeText);
        if(sensor==NULL)
        {
            continue;
        }
        SensorParser *sensorParser=ParserManager::instance()->getSensorParserByTypeText(sensorTypeText);
        if(sensorParser==NULL)
        {
            continue;
        }
        sensorParser->parseDBDataToSensor(query, sensor);
        sensorMap->insert(sensor->sensorId(), sensor);
    }
    return sensorMap;
}

void asyncSetControlDateList(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, quint8 groupId, QList<ControlDate*> *controlDateList)
{
    QMutexLocker locker(dbWriteMutex);
    dbWriteHolder->getDb().transaction();
    int len=controlDateList->size();
    QSqlQuery query;
    for(int i=0; i<len; i++)
    {
        ControlDate *controlDate=controlDateList->at(i);
        query=dbWriteHolder->getDb().exec(QString("replace into control_date(id, group_id, month, day, policy_id) values('%1', %2, %3, %4, %5)")
                                          .arg(controlDate->id)
                                          .arg(controlDate->groupId&0xFF)
                                          .arg(controlDate->month)
                                          .arg(controlDate->day)
                                          .arg(controlDate->controlPolicyId));
        delete controlDate;
        if(!query.lastError().text().trimmed().isEmpty())
        {
            break;
        }
    }
    dbWriteHolder->getDb().commit();
    delete controlDateList;

    qDebug()<<"asyncSetControlDates, error="<<query.lastError().text();
}

QList<ControlDate*> *asyncGetControlDateList(MeshDBHolder *dbReadHolder)
{
    QSqlQuery query=dbReadHolder->getDb().exec(QString("select * from control_date"));
    qDebug()<<"asyncGetControlDateList, error="<<query.lastError().text();
    QList<ControlDate*> *controlDateList=new QList<ControlDate*>;
    while(query.next())
    {
        ControlDate *controlDate=new ControlDate;
        controlDate->id=query.value("id").toString();
        controlDate->groupId=query.value("group_id").toInt();
        controlDate->month=query.value("month").toInt();
        controlDate->day=query.value("day").toInt();
        controlDate->controlPolicyId=query.value("policy_id").toInt();
        controlDateList->push_back(controlDate);
    }
    return controlDateList;

}

void asyncDeleteExecutersConfigData(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, QList<QString> *executerIdList)
{
    QMutexLocker locker(dbWriteMutex);
    dbWriteHolder->getDb().transaction();
    for(int i=0; i<executerIdList->size(); i++)
    {
        QSqlQuery deleteExecuterQuery(dbWriteHolder->getDb());
        deleteExecuterQuery.exec(QString("delete from executer_node where executer_id='%1'").arg(executerIdList->at(i)));
    }
    dbWriteHolder->getDb().commit();
    delete executerIdList;
}

void asyncDeleteSensorsConfigData(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, QList<QString> *sensorIdList)
{
    QMutexLocker locker(dbWriteMutex);
    dbWriteHolder->getDb().transaction();
    for(int i=0; i<sensorIdList->size(); i++)
    {
        QSqlQuery deleteSensorQuery(dbWriteHolder->getDb());
        deleteSensorQuery.exec(QString("delete from sensor_node where sensor_id='%1'").arg(sensorIdList->at(i)));
    }
    dbWriteHolder->getDb().commit();
    delete sensorIdList;
}

void asyncSaveTemplateItemListAsNewCollection(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex,
                                              TemplateTimeLineControlPolicy *policy,
                                              QList<TimeLineControlItem*> *itemList)
{
    QMutexLocker locker(dbWriteMutex);
    bool bOk = false;
    QString strCmd = "";

    dbWriteHolder->getDb().transaction();
    QSqlQuery addControlPolicyQuery(dbWriteHolder->getDb());
    strCmd = QString("insert into template_control_policy(policy_id, policy_name, group_type, create_msec)values('%1', '%2', '%3', %4)")
            .arg(policy->id)
            .arg(policy->name)
            .arg(policy->groupType)
            .arg(QDateTime::currentMSecsSinceEpoch());
    bOk = addControlPolicyQuery.exec(strCmd);

    if (!bOk)
    {
        qWarning() << "In MeshDbCaller: asyncSaveTemplateItemListAsNewCollection() execute SQL query failed:"
                   << "SQL=" << strCmd << "Error=" << addControlPolicyQuery.lastError().text();
    }

    while(!itemList->isEmpty())
    {
        TimeLineControlItem *controlItem = itemList->takeFirst();
        QSqlQuery addControlItemQuery(dbWriteHolder->getDb());
        addControlItemQuery.exec(QString("insert into template_control_item(item_id, policy_id, start_minute, control_type)values('%1', '%2', %3, '%4')")
                                 .arg(controlItem->templateItemId)
                                 .arg(controlItem->templatePolicyId)
                                 .arg(controlItem->startMinute)
                                 .arg(controlItem->controlType));

        QList<QPair<QString, QString> > propertyPairList;
        controlItem->controlDataToPropertyPairList(&propertyPairList);

        for(int i=0; i<propertyPairList.size(); i++)
        {
            QPair<QString, QString> propertyPair=propertyPairList.at(i);
            QSqlQuery addControlItemPropertyQuery(dbWriteHolder->getDb());

            strCmd = QString("replace into template_control_item_property(item_id, property_name, property_value)values('%1', '%2', '%3')")
                    .arg(controlItem->templateItemId)
                    .arg(propertyPair.first)
                    .arg(propertyPair.second);
            bOk = addControlItemPropertyQuery.exec(strCmd);

            if (!bOk)
            {
                qWarning() << "In MeshDbCaller: asyncSaveTemplateItemListAsNewCollection() execute SQL query failed:"
                           << "SQL=" << strCmd << "Error=" << addControlItemPropertyQuery.lastError().text();
            }
        }

        delete controlItem;
    }
    dbWriteHolder->getDb().commit();
    delete itemList;
}

QList<TemplateTimeLineControlPolicy*> *asyncLoadTemplateTimeLineControlPolicyList(MeshDBHolder *dbReadHolder, NodeGroup *group = nullptr)
{
    //Use group_type to filter the control_policies
    QSqlQuery loadPolicyListQuery(dbReadHolder->getDb());
    QString strCmd = "select * from template_control_policy";
    if (group != nullptr)
    {
        strCmd.append(QString(" where group_type='%1'").arg(NodeGroup::getTextByGroupType(group->getGroupType())));
    }

    loadPolicyListQuery.exec(strCmd);
    QList<TemplateTimeLineControlPolicy*> *policyList=new QList<TemplateTimeLineControlPolicy*>;
    while(loadPolicyListQuery.next())
    {
        TemplateTimeLineControlPolicy *policy=new TemplateTimeLineControlPolicy;
        policy->id=loadPolicyListQuery.value("policy_id").toString();
        policy->name=loadPolicyListQuery.value("policy_name").toString();
        policy->groupType=loadPolicyListQuery.value("group_type").toString();
        policyList->push_back(policy);
    }
    return policyList;
}

void asyncAddControlItemsFromTemplate(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, NodeGroup *group, int subId, QList<TimeLineControlItem *> *templateItemList)
{
    QMutexLocker locker(dbWriteMutex);

    while(!templateItemList->isEmpty())
    {
        TimeLineControlItem *controlItem = templateItemList->takeFirst();
        QJsonObject *controlDataJson = new QJsonObject;
        controlItem->controlDataToJson(controlDataJson);
        QString controlDataText = QString::fromUtf8(Tools::getDataArrayOfJsonObject(controlDataJson));

        QString cmd=QString("insert into control_item("
                            "id, "
                            "group_id, "
                            "policy_sub_id, "
                            "control_data) values(%1, %2, %3, '%4')")
                .arg((group->getId()&0xFF)*10000*100+((int)subId)*10000+controlItem->startMinute)
                .arg((quint16)group->getId())
                .arg(subId)
                .arg(controlDataText);
        QSqlQuery query=dbWriteHolder->getDb().exec(cmd);
        qDebug()<<"asyncAddControlItemsFromTemplate, error="<<query.lastError().text();

        delete controlItem;
        delete controlDataJson;
    }
}

void asyncUpdateTemplateTimeLineControlPolicy(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, TemplateTimeLineControlPolicy *policy)
{
    QMutexLocker locker(dbWriteMutex);
    QSqlQuery updateQuery(dbWriteHolder->getDb());
    QString strCmd = QString("update template_control_policy set policy_name='%1', group_type='%2' where policy_id='%3'")
            .arg(policy->name)
            .arg(policy->groupType)
            .arg(policy->id);
    bool bOk = updateQuery.exec(strCmd);
    if (!bOk)
    {
        qWarning() << "In MeshDbCaller: asyncUpdateTemplateTimeLineControlPolicy() execute SQL query failed:"
                   << "SQL=" << strCmd << "Error=" << updateQuery.lastError().text();
    }
}

TemplateTimeLineControlPolicy *asyncLoadTemplateTimeLineControlPolicy(MeshDBHolder *dbReadHolder, QString policyId)
{
    QSqlQuery loadQuery(dbReadHolder->getDb());
    loadQuery.exec(QString("select * from template_control_policy where policy_id='%1'").arg(policyId));
    TemplateTimeLineControlPolicy *policy=new TemplateTimeLineControlPolicy;
    if(loadQuery.next())
    {
        policy->id=loadQuery.value("policy_id").toString();
        policy->name=loadQuery.value("policy_name").toString();
        policy->groupType=loadQuery.value("group_type").toString();
    }
    return policy;
}

QList<TimeLineControlItem*> *asyncLoadTemplateTimeLineControlItemList(MeshDBHolder *dbReadHolder, QString policyId)
{
    QString strGroupType = "";

    QSqlQuery controlPolicyQuery(dbReadHolder->getDb());
    controlPolicyQuery.exec(QString("select group_type from template_control_policy where policy_id='%1'").arg(policyId));
    if (controlPolicyQuery.next())
    {
        strGroupType = controlPolicyQuery.value("group_type").toString();
    }
    else
    {
        qDebug() << "asyncLoadTemplateTimeLineControlItemList:"
                 << "No such policy found";
        return nullptr;
    }

    QSqlQuery controlItemListQuery(dbReadHolder->getDb());
    controlItemListQuery.exec(QString("select * from template_control_item where policy_id='%1' order by start_minute asc").arg(policyId));
    QList<TimeLineControlItem*> *itemList=new QList<TimeLineControlItem*>;
    while(controlItemListQuery.next())
    {
        TimeLineControlItem *item = ObjectFactory::instance()->createTimeLineControlItemByGroupTypeText(strGroupType);

        item->templateItemId = controlItemListQuery.value("item_id").toString();
        item->templatePolicyId = controlItemListQuery.value("policy_id").toString();


        item->startMinute = controlItemListQuery.value("start_minute").toInt();
        item->controlType = controlItemListQuery.value("control_type").toString();

        QMap<QString, QString> propertyMap;
        QSqlQuery controlItemPropertyQuery(dbReadHolder->getDb());
        controlItemPropertyQuery.exec(QString("select property_name, property_value from template_control_item_property where item_id='%1'").arg(item->templateItemId));
        while(controlItemPropertyQuery.next())
        {
            propertyMap.insert(controlItemPropertyQuery.value("property_name").toString(),
                               controlItemPropertyQuery.value("property_value").toString());
        }

        item->controlDataFromPropertyPairList(&propertyMap);
        itemList->push_back(item);
    }
    return itemList;
}

void asyncAddTemplateTimeLineControlItem(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, TimeLineControlItem *item)
{
    QMutexLocker locker(dbWriteMutex);
    QSqlQuery addControlItemQuery(dbWriteHolder->getDb());
    addControlItemQuery.exec(QString("insert into template_control_item(item_id, policy_id, start_minute, control_type)values('%1','%2',%3,'%4')")
                             .arg(item->templateItemId)
                             .arg(item->templatePolicyId)
                             .arg(item->startMinute)
                             .arg(item->controlType));
}

void asyncUpdateTemplateTimeLineControlItem(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, TimeLineControlItem *controlItem)
{
    QMutexLocker locker(dbWriteMutex);
    dbWriteHolder->getDb().transaction();
    QSqlQuery updateControlItemQuery(dbWriteHolder->getDb());
    updateControlItemQuery.exec(QString("update template_control_item set control_type='%1' where item_id='%2'")
                                .arg(controlItem->controlType)
                                .arg(controlItem->templateItemId));

    QList<QPair<QString, QString> > propertyPairList;
    controlItem->controlDataToPropertyPairList(&propertyPairList);

    for(int i=0; i<propertyPairList.size(); i++)
    {
        QPair<QString, QString> propertyPair=propertyPairList.at(i);
        QSqlQuery upodateControlItemPropertyQuery(dbWriteHolder->getDb());
        upodateControlItemPropertyQuery.exec(QString("replace into template_control_item_property(item_id, property_name, property_value)values('%1', '%2', '%3')")
                        .arg(controlItem->templateItemId)
                        .arg(propertyPair.first)
                        .arg(propertyPair.second));
    }

    dbWriteHolder->getDb().commit();
    delete controlItem;
    controlItem = nullptr;
}

TimeLineControlItem *asyncLoadTemplateTimeLineControlItem(MeshDBHolder *dbReadHolder, QString itemId)
{
    QSqlQuery controlItemQuery(dbReadHolder->getDb());
    controlItemQuery.exec(QString("select * from template_control_item where item_id='%1'").arg(itemId));
    TimeLineControlItem *item = nullptr;
    if(controlItemQuery.next())
    {
        QString strPolicyId = controlItemQuery.value("policy_id").toString();
        QString strType = "";
        QSqlQuery groupTypeQuery(dbReadHolder->getDb());
        groupTypeQuery.exec(QString("select group_type from template_control_policy where policy_id='%1'").arg(strPolicyId));
        if (groupTypeQuery.next())
        {
            strType = groupTypeQuery.value("group_type").toString();
        }
        else
        {
            qDebug() << "asyncLoadTemplateTimeLineControlItem:"
                     << "No such policy found for policy_id:"
                     << item->templatePolicyId;
            return nullptr;
        }

        item = ObjectFactory::instance()->createTimeLineControlItemByGroupTypeText(strType);
        item->templateItemId = controlItemQuery.value("item_id").toString();
        item->templatePolicyId = controlItemQuery.value("policy_id").toString();
        item->startMinute = controlItemQuery.value("start_minute").toInt();
        item->controlType = controlItemQuery.value("control_type").toString();

        QMap<QString, QString> propertyMap;
        QSqlQuery controlItemPropertyQuery(dbReadHolder->getDb());
        controlItemPropertyQuery.exec(QString("select property_name, property_value from template_control_item_property where item_id='%1'").arg(item->templateItemId));
        while(controlItemPropertyQuery.next())
        {
            propertyMap.insert(controlItemPropertyQuery.value("property_name").toString(),
                               controlItemPropertyQuery.value("property_value").toString());
        }

        item->controlDataFromPropertyPairList(&propertyMap);
    }
    return item;
}

void asyncRemoveTemplateTimeLineControlItem(MeshDBHolder *dbWriteHolder, QMutex *dbWriteMutex, QString itemId)
{
    QMutexLocker locker(dbWriteMutex);
    dbWriteHolder->getDb().transaction();
    QSqlQuery removeControlItemPropertyQuery(dbWriteHolder->getDb());
    removeControlItemPropertyQuery.exec(QString("delete from template_control_item_property where item_id='%1'").arg(itemId));
    QSqlQuery removeControlItemQuery(dbWriteHolder->getDb());
    removeControlItemQuery.exec(QString("delete from template_control_item where item_id='%1'").arg(itemId));
    dbWriteHolder->getDb().commit();
}

MeshDbCaller::MeshDbCaller(const QString &meshUUID, QObject *parent) :
    QObject(parent)
{
    dbReadOnlyHolder=new MeshDBHolder(meshUUID);
    dbReadWriteHolder=new MeshDBHolder(meshUUID);
    dbReadWriteHolder->initDB(true);
    dbReadOnlyHolder->initDB(false);
    dbWriteMutex=new QMutex;
}

MeshDbCaller::~MeshDbCaller()
{
    if(dbReadOnlyHolder!=NULL)
    {
        delete dbReadOnlyHolder;
        dbReadOnlyHolder=NULL;
    }
    if(dbReadWriteHolder!=NULL)
    {
        delete dbReadWriteHolder;
        dbReadWriteHolder=NULL;
    }
    if(dbWriteMutex!=NULL)
    {
        delete dbWriteMutex;
        dbWriteMutex = NULL;
    }
}

void MeshDbCaller::addNodeGroup(NodeGroup *nodeGroup)
{
    qDebug()<<"MeshDbCaller::addNodeGroup groupId="<<(quint16)nodeGroup->getId()<<",name="<<nodeGroup->getName()<<",color="<<nodeGroup->getColor();
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doAddNodeGroupTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncAddNodeGroup, dbReadWriteHolder, dbWriteMutex, nodeGroup);
    watcher->setFuture(future);
}

void MeshDbCaller::removeNodeGroup(quint8 groupId)
{
    qDebug()<<"MeshDbCaller::removeNodeGroup groupId="<<(quint16)groupId;
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doRemoveNodeGroupTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncRemoveNodeGroup, dbReadWriteHolder, dbWriteMutex, groupId);
    watcher->setFuture(future);
}

void MeshDbCaller::syncNodeGroups()
{
    qDebug()<<"MeshDbCaller::syncNodeGroups";
    QFutureWatcher<QList<NodeGroup*>*> *watcher=new QFutureWatcher<QList<NodeGroup*>*>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doSyncNodeGroupsTaskFinished()));
    QFuture<QList<NodeGroup*>*> future=QtConcurrent::run(asyncGetNodeGroups, dbReadOnlyHolder);
    watcher->setFuture(future);
}

void MeshDbCaller::syncControlPolicies()
{
    qDebug()<<"MeshDbCaller::syncControlPolicies";
    QFutureWatcher<QList<TimeLineControlPolicy*>*> *watcher=new QFutureWatcher<QList<TimeLineControlPolicy*>*>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doSyncControlPoliciesTaskFinished()));
    QFuture<QList<TimeLineControlPolicy*>*> future=QtConcurrent::run(asyncGetControlPolicies, dbReadOnlyHolder);
    watcher->setFuture(future);
}

void MeshDbCaller::addControlPolicy(TimeLineControlPolicy *controlPolicy)
{
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    int policyId=(controlPolicy->groupId&0xFF)*100+controlPolicy->subId;
    controlPolicy->id=policyId;
    connect(watcher, SIGNAL(finished()), this, SLOT(doAddControlPolicyTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncAddControlPolicy, dbReadWriteHolder, dbWriteMutex, controlPolicy);
    watcher->setFuture(future);
}

void MeshDbCaller::updateControlPolicy(TimeLineControlPolicy *controlPolicy)
{
    qDebug()<<"MeshDbCaller::updateControlPolicy controlPolicy.id="<<controlPolicy->id;
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    watcher->setProperty("id", controlPolicy->id);
    connect(watcher, SIGNAL(finished()), this, SLOT(doUpdateControlPolicyTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncUpdateControlPolicy, dbReadWriteHolder, dbWriteMutex, controlPolicy);
    watcher->setFuture(future);
}

void MeshDbCaller::removeControlPolicy(int policyId)
{
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doRemoveControlPolicyTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncRemoveControlPolicy, dbReadWriteHolder, dbWriteMutex, policyId);
    watcher->setFuture(future);
}


void MeshDbCaller::syncControlPolicy(int controlPolicyId)
{
    qDebug()<<"MeshDbCaller::syncControlPolicy controlPolicyId="<<controlPolicyId;
    QFutureWatcher<TimeLineControlPolicy*> *watcher=new QFutureWatcher<TimeLineControlPolicy*>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doSyncControlPolicyTaskFinished()));
    QFuture<TimeLineControlPolicy*> future=QtConcurrent::run(asyncGetControlPolicy, dbReadOnlyHolder, controlPolicyId);
    watcher->setFuture(future);
}

void MeshDbCaller::addControlItem(TimeLineControlItem *controlItem)
{
    qDebug()<<"MeshDbCaller::addControlItem controlItem.startMinute="<<controlItem->startMinute;
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doAddControlItemTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncAddControlItem, dbReadWriteHolder, dbWriteMutex, controlItem);
    watcher->setFuture(future);
}

void MeshDbCaller::syncControlItems()
{
    qDebug()<<"MeshDbCaller::syncControlItems";
    QFutureWatcher<QList<TimeLineControlItem*>*> *watcher=new QFutureWatcher<QList<TimeLineControlItem*>*>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doSyncControlItemsTaskFinished()));
    QFuture<QList<TimeLineControlItem*>*> future=QtConcurrent::run(asyncGetControlItems, dbReadOnlyHolder);
    watcher->setFuture(future);
}

void MeshDbCaller::updateControlItem(TimeLineControlItem *controlItem)
{
    qDebug()<<"MeshDbCaller::updateControlItem controlItem.id="<<controlItem->id;
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    watcher->setProperty("id", controlItem->id);
    connect(watcher, SIGNAL(finished()), this, SLOT(doUpdateControlItemTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncUpdateControlItem, dbReadWriteHolder, dbWriteMutex, controlItem);
    watcher->setFuture(future);
}

void MeshDbCaller::removeControlItem(int itemId)
{
    qDebug()<<"MeshDbCaller::removeControlItem itemId="<<itemId;
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    watcher->setProperty("id", itemId);
    connect(watcher, SIGNAL(finished()), this, SLOT(doRemoveControlItemTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncRemoveControlItem, dbReadWriteHolder, dbWriteMutex, itemId);
    watcher->setFuture(future);
}

void MeshDbCaller::syncControlItem(int itemId)
{
    qDebug()<<"MeshDbCaller::syncControlItem itemId="<<itemId;
    QFutureWatcher<TimeLineControlItem*> *watcher=new QFutureWatcher<TimeLineControlItem*>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doSyncControlItemTaskFinished()));
    QFuture<TimeLineControlItem*> future=QtConcurrent::run(asyncGetControlItem, dbReadOnlyHolder, itemId);
    watcher->setFuture(future);
}

void MeshDbCaller::setExecuterNodesName(QMap<QString, QPair<QString, QString> > *nameMap)
{
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doSetExecuterNodesNameTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncSetExecuterNodesName, dbReadWriteHolder, dbWriteMutex, nameMap);
    watcher->setFuture(future);
}

void MeshDbCaller::setSensorNodesName(QMap<QString, QPair<QString, QString> > *nameMap)
{
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doSetSensorNodesNameTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncSetSensorNodesName, dbReadWriteHolder, dbWriteMutex, nameMap);
    watcher->setFuture(future);
}

void MeshDbCaller::addSensor(Sensor *sensor)
{
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doAddSensorTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncAddSensor, dbReadWriteHolder, dbWriteMutex, sensor);
    watcher->setFuture(future);
}

void MeshDbCaller::updateSensor(Sensor *sensor)
{
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doUpdateSensorTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncUpdateSensor, dbReadWriteHolder, dbWriteMutex, sensor);
    watcher->setFuture(future);
}

void MeshDbCaller::removeSensor(const QString &sensorId)
{
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doRemoveSensorTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncRemoveSensor, dbReadWriteHolder, dbWriteMutex, sensorId);
    watcher->setFuture(future);
}

void MeshDbCaller::setNodeGroupsName(QMap<quint8, QString> *nameMap)
{
    qDebug()<<"MeshDbCaller::setNodeGroupsName nameMap.size="<<nameMap->size();
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doSetNodeGroupsNameTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncSetNodeGroupsName, dbReadWriteHolder, dbWriteMutex, nameMap);
    watcher->setFuture(future);
}

void MeshDbCaller::updateNodeGroupsScheduleData(NodeGroup *nodeGroup)
{
    qDebug()<<"MeshDbCaller::updateNodeGroupsScheduleData scheduleData="<<nodeGroup->buildGroupData();
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doUpdateNodeGroupsScheduleDataTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncUpdateNodeGroupScheduleData, dbReadWriteHolder, dbWriteMutex, nodeGroup);
    watcher->setFuture(future);
}

void MeshDbCaller::syncExecuterNodes()
{
    QFutureWatcher<QMap<QString, Executer*>*> *watcher=new QFutureWatcher<QMap<QString, Executer*>*>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doSyncExecuterNodesTaskFinished()));
    QFuture<QMap<QString, Executer*>*> future=QtConcurrent::run(asyncGetExecuterNodes, dbReadOnlyHolder);
    watcher->setFuture(future);
}

void MeshDbCaller::syncSensorNodes()
{
    qDebug()<<"MeshDbCaller::syncSensorNodes";
    QFutureWatcher<QMap<QString, Sensor*>*> *watcher=new QFutureWatcher<QMap<QString, Sensor*>*>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doSyncSensorNodesTaskFinished()));
    QFuture<QMap<QString, Sensor*>*> future=QtConcurrent::run(asyncGetSensorNodes, dbReadOnlyHolder);
    watcher->setFuture(future);
}

void MeshDbCaller::setControlDates(quint8 groupId, QList<ControlDate *> *controlDateList)
{
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doSetControlDatesTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncSetControlDateList, dbReadWriteHolder, dbWriteMutex, groupId, controlDateList);
    watcher->setFuture(future);
}

void MeshDbCaller::syncControlDates()
{
    qDebug()<<"MeshDbCaller::syncControlDates";
    QFutureWatcher<QList<ControlDate*>*> *watcher=new QFutureWatcher<QList<ControlDate*>*>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doSyncControlDatesTaskFinished()));
    QFuture<QList<ControlDate*>*> future=QtConcurrent::run(asyncGetControlDateList, dbReadOnlyHolder);
    watcher->setFuture(future);
}

void MeshDbCaller::deleteExecutersConfigData(QList<QString> *executerIdList)
{
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doAsyncDeleteExecutersConfigDataFinished()));
    QFuture<void> future=QtConcurrent::run(asyncDeleteExecutersConfigData, dbReadWriteHolder, dbWriteMutex, executerIdList);
    watcher->setFuture(future);
}

void MeshDbCaller::deleteSensorsConfigData(QList<QString> *sensorIdList)
{
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doAsyncDeleteSensorsConfigDataFinished()));
    QFuture<void> future=QtConcurrent::run(asyncDeleteSensorsConfigData, dbReadWriteHolder, dbWriteMutex, sensorIdList);
    watcher->setFuture(future);
}

void MeshDbCaller::saveTemplateItemListAsNewCollection(TemplateTimeLineControlPolicy *policy, QList<TimeLineControlItem *> *itemList)
{
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doSaveTemplateItemListAsNewCollectionTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncSaveTemplateItemListAsNewCollection, dbReadWriteHolder, dbWriteMutex, policy, itemList);
    watcher->setFuture(future);
}

void MeshDbCaller::doAddNodeGroupTaskFinished()
{
    qDebug()<<"MeshDbCaller::doAddNodeGroupTaskFinished";
    syncNodeGroups();
    syncControlPolicies();
}

void MeshDbCaller::doRemoveNodeGroupTaskFinished()
{
    qDebug()<<"MeshDbCaller::doRemoveNodeGroupTaskFinished";
    syncNodeGroups();
    syncControlItems();
}

void MeshDbCaller::doSyncNodeGroupsTaskFinished()
{
    qDebug()<<"MeshDbCaller::doSyncNodeGroupsTaskFinished";
    QFutureWatcher<QList<NodeGroup*>*> *watcher=static_cast<QFutureWatcher<QList<NodeGroup*>*>*>(sender());
    if(watcher==NULL)
    {
        qDebug()<<"MeshDbCaller::doSyncNodeGroupsTaskFinished watcher=NULL";
    }
    else
    {
        QList<NodeGroup*> *groupList=watcher->result();
        emit nodeGroupsChanged(groupList);
    }
}

void MeshDbCaller::doSyncControlPoliciesTaskFinished()
{
    qDebug()<<"MeshDbCaller::doSyncControlPoliciesTaskFinished";
    QFutureWatcher<QList<TimeLineControlPolicy*>*> *watcher=static_cast<QFutureWatcher<QList<TimeLineControlPolicy*>*>*>(sender());
    if(watcher==NULL)
    {
        qDebug()<<"MeshDbCaller::doSyncControlPoliciesTaskFinished watcher=NULL";
    }
    else
    {
        QList<TimeLineControlPolicy*> *controlPolicyList=watcher->result();
        emit controlPoliciesChanged(controlPolicyList);
    }
}

void MeshDbCaller::doAddControlPolicyTaskFinished()
{
    QFutureWatcher<void> *watcher=static_cast<QFutureWatcher<void>*>(sender());
    if(watcher!=NULL)
    {
        syncControlPolicies();
        watcher->deleteLater();
    }
}

void MeshDbCaller::doUpdateControlPolicyTaskFinished()
{
    qDebug()<<"MeshDbCaller::doUpdateControlPolicyTaskFinished";
    int id=sender()->property("id").toInt();
    syncControlPolicy(id);
}

void MeshDbCaller::doRemoveControlPolicyTaskFinished()
{
    QFutureWatcher<void> *watcher=static_cast<QFutureWatcher<void>*>(sender());
    if(watcher!=NULL)
    {
        syncControlPolicies();
        syncControlItems();
        watcher->deleteLater();
    }
}

void MeshDbCaller::doSyncControlPolicyTaskFinished()
{
    qDebug()<<"MeshDbCaller::doSyncControlPolicyTaskFinished";
    QFutureWatcher<TimeLineControlPolicy*> *watcher=static_cast<QFutureWatcher<TimeLineControlPolicy*>*>(sender());
    if(watcher==NULL)
    {
        qDebug()<<"MeshDbCaller::doSyncControlPolicyTaskFinished watcher=NULL";
    }
    else
    {
        TimeLineControlPolicy *controlPolicy=watcher->result();
        emit controlPolicyChanged(controlPolicy);
    }
}

void MeshDbCaller::doAddControlItemTaskFinished()
{
    qDebug()<<"MeshDbCaller::doAddControlItemTaskFinished";
    syncControlItems();
}

void MeshDbCaller::doSyncControlItemsTaskFinished()
{
    qDebug()<<"MeshDbCaller::doSyncControlItemsTaskFinished";
    QFutureWatcher<QList<TimeLineControlItem*>*> *watcher=static_cast<QFutureWatcher<QList<TimeLineControlItem*>*>*>(sender());
    if(watcher==NULL)
    {
        qDebug()<<"MeshDbCaller::doSyncControlItemsTaskFinished watcher=NULL";
    }
    else
    {
        QList<TimeLineControlItem*> *itemList=watcher->result();
        emit controlItemsChanged(itemList);
    }
}

void MeshDbCaller::doUpdateControlItemTaskFinished()
{
    qDebug()<<"MeshDbCaller::doUpdateControlItemTaskFinished";
    int id=sender()->property("id").toInt();
    syncControlItem(id);
}

void MeshDbCaller::doRemoveControlItemTaskFinished()
{
    qDebug()<<"MeshDbCaller::doRemoveControlItemTaskFinished";
    syncControlItems();
}

void MeshDbCaller::doSyncControlItemTaskFinished()
{
    qDebug()<<"MeshDbCaller::doSyncControlItemTaskFinished";
    QFutureWatcher<TimeLineControlItem*> *watcher=static_cast<QFutureWatcher<TimeLineControlItem*>*>(sender());
    if(watcher==NULL)
    {
        qDebug()<<"MeshDbCaller::doSyncControlItemTaskFinished watcher=NULL";
    }
    else
    {
        TimeLineControlItem *item=watcher->result();
        emit controlItemChanged(item);
    }
}

void MeshDbCaller::doSetExecuterNodesNameTaskFinished()
{
    QFutureWatcher<void> *watcher=static_cast<QFutureWatcher<void>*>(sender());
    if(watcher!=NULL)
    {
        syncExecuterNodes();
        watcher->deleteLater();
    }
}

void MeshDbCaller::doSetSensorNodesNameTaskFinished()
{
    QFutureWatcher<void> *watcher=static_cast<QFutureWatcher<void>*>(sender());
    if(watcher!=NULL)
    {
        syncSensorNodes();
        watcher->deleteLater();
    }
}

void MeshDbCaller::doUpdateNodeGroupsScheduleDataTaskFinished()
{
    QFutureWatcher<void> *watcher=static_cast<QFutureWatcher<void>*>(sender());
    if(watcher!=NULL)
    {
        syncNodeGroups();
        watcher->deleteLater();
    }
}

void MeshDbCaller::doAddSensorTaskFinished()
{
    QFutureWatcher<void> *watcher=static_cast<QFutureWatcher<void>*>(sender());
    if(watcher!=NULL)
    {
        syncSensorNodes();
        watcher->deleteLater();
    }
}

void MeshDbCaller::doUpdateSensorTaskFinished()
{
    QFutureWatcher<void> *watcher=static_cast<QFutureWatcher<void>*>(sender());
    if(watcher!=NULL)
    {
        syncSensorNodes();
        watcher->deleteLater();
    }
}

void MeshDbCaller::doRemoveSensorTaskFinished()
{
    QFutureWatcher<void> *watcher=static_cast<QFutureWatcher<void>*>(sender());
    if(watcher!=NULL)
    {
        syncSensorNodes();
        watcher->deleteLater();
    }
}

void MeshDbCaller::doSetNodeGroupsNameTaskFinished()
{
    QFutureWatcher<void> *watcher=static_cast<QFutureWatcher<void>*>(sender());
    if(watcher!=NULL)
    {
        syncNodeGroups();
        watcher->deleteLater();
    }
}

void MeshDbCaller::doSyncExecuterNodesTaskFinished()
{
    QFutureWatcher<QMap<QString, Executer*>*> *watcher=static_cast<QFutureWatcher<QMap<QString, Executer*>*>*>(sender());
    if(watcher!=NULL)
    {
        QMap<QString, Executer*> *nameMap=watcher->result();
        emit executerNodesChanged(nameMap);
        watcher->deleteLater();
    }
}

void MeshDbCaller::doSyncSensorNodesTaskFinished()
{
    qDebug()<<"MeshDbCaller::doSyncSensorNodesTaskFinished";
    QFutureWatcher<QMap<QString, Sensor*>*> *watcher=static_cast<QFutureWatcher<QMap<QString, Sensor*>*>*>(sender());
    if(watcher==NULL)
    {
        qDebug()<<"MeshDbCaller::doSyncSensorNodesTaskFinished watcher=NULL";
    }
    else
    {
        QMap<QString, Sensor*> *sensorMap=watcher->result();
        emit sensorNodesChanged(sensorMap);
    }
}

void MeshDbCaller::doSetControlDatesTaskFinished()
{
    qDebug()<<"MeshDbCaller::doSetControlDatesTaskFinished";
    QFutureWatcher<void> *watcher=static_cast<QFutureWatcher<void>*>(sender());
    if(watcher!=NULL)
    {
        syncControlDates();
        watcher->deleteLater();
    }
}

void MeshDbCaller::doSyncControlDatesTaskFinished()
{
    qDebug()<<"MeshDbCaller::doSyncControlDatesTaskFinished";
    QFutureWatcher<QList<ControlDate*>*> *watcher=static_cast<QFutureWatcher<QList<ControlDate*>*>*>(sender());
    if(watcher!=NULL)
    {
        QList<ControlDate*> *controlDateList=watcher->result();
        emit controlDatesChanged(controlDateList);
        watcher->deleteLater();
    }
}

void MeshDbCaller::doAsyncDeleteExecutersConfigDataFinished()
{
    QFutureWatcher<void> *watcher=static_cast<QFutureWatcher<void>*>(sender());
    if(watcher!=NULL)
    {
        syncExecuterNodes();
        watcher->deleteLater();
    }
}

void MeshDbCaller::doAsyncDeleteSensorsConfigDataFinished()
{
    QFutureWatcher<void> *watcher=static_cast<QFutureWatcher<void>*>(sender());
    if(watcher!=NULL)
    {
        syncSensorNodes();
        watcher->deleteLater();
    }
}

void MeshDbCaller::doSaveTemplateItemListAsNewCollectionTaskFinished()
{
    QFutureWatcher<void> *watcher=static_cast<QFutureWatcher<void>*>(sender());
    if(watcher!=NULL)
    {
        emit saveTemplateItemListAsNewCollectionFinished();
        loadTemplateTimeLineControlPolicyList();
        watcher->deleteLater();
    }
}

void MeshDbCaller::loadTemplateTimeLineControlPolicyList(NodeGroup *group)
{
    QFutureWatcher<QList<TemplateTimeLineControlPolicy*>*> *watcher=new QFutureWatcher<QList<TemplateTimeLineControlPolicy*>*>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doLoadTemplateTimeLineControlPolicyListTaskFinished()));
    QFuture<QList<TemplateTimeLineControlPolicy*>*> future=QtConcurrent::run(asyncLoadTemplateTimeLineControlPolicyList, dbReadOnlyHolder, group);
    watcher->setFuture(future);
}

void MeshDbCaller::doLoadTemplateTimeLineControlPolicyListTaskFinished()
{
    QFutureWatcher<QList<TemplateTimeLineControlPolicy*>*> *watcher=static_cast<QFutureWatcher<QList<TemplateTimeLineControlPolicy*>*>*>(sender());
    if(watcher!=NULL)
    {
        QList<TemplateTimeLineControlPolicy*> *policyList=watcher->result();
        emit templateTimeLineControlPolicyListLoaded(policyList);
        watcher->deleteLater();
    }
}

void MeshDbCaller::addControlItemsFromTemplate(NodeGroup *group, int subId, QList<TimeLineControlItem *> *templateItemList)
{
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doAddControlItemsFromTemplateTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncAddControlItemsFromTemplate, dbReadWriteHolder, dbWriteMutex, group, subId, templateItemList);
    watcher->setFuture(future);
}

void MeshDbCaller::doAddControlItemsFromTemplateTaskFinished()
{
    QFutureWatcher<void> *watcher=static_cast<QFutureWatcher<void>*>(sender());
    if(watcher!=NULL)
    {
        emit addControlItemsFromTemplateFinshed();
        syncControlItems();
        watcher->deleteLater();
    }
}

void MeshDbCaller::addTemplateTimeLineControlPolicy(TemplateTimeLineControlPolicy *policy)
{
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doAddTemplateTimeLineControlPolicyTaskFinished()));
    policy->id=Tools::getRandomUUID();
    QFuture<void> future=QtConcurrent::run(asyncAddTemplateTimeLineControlPolicy, dbReadWriteHolder, dbWriteMutex, policy);
    watcher->setFuture(future);
}

void MeshDbCaller::doAddTemplateTimeLineControlPolicyTaskFinished()
{
    QFutureWatcher<void> *watcher=static_cast<QFutureWatcher<void>*>(sender());
    if(watcher!=NULL)
    {
        loadTemplateTimeLineControlPolicyList();
        watcher->deleteLater();
    }
}

void MeshDbCaller::removeTemplateTimeLineControlPolicy(QString policyId)
{
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doRemoveTemplateTimeLineControlPolicyTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncRemoveTemplateTimeLineControlPolicy, dbReadWriteHolder, dbWriteMutex, policyId);
    watcher->setFuture(future);
}

void MeshDbCaller::doRemoveTemplateTimeLineControlPolicyTaskFinished()
{
    QFutureWatcher<void> *watcher=static_cast<QFutureWatcher<void>*>(sender());
    if(watcher!=NULL)
    {
        loadTemplateTimeLineControlPolicyList();
        watcher->deleteLater();
    }
}

void MeshDbCaller::updateTemplateTimeLineControlPolicy(TemplateTimeLineControlPolicy *policy)
{
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    watcher->setProperty("policy_id", policy->id);
    connect(watcher, SIGNAL(finished()), this, SLOT(doUpdateTemplateTimeLineControlPolicyTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncUpdateTemplateTimeLineControlPolicy, dbReadWriteHolder, dbWriteMutex, policy);
    watcher->setFuture(future);
}

void MeshDbCaller::doUpdateTemplateTimeLineControlPolicyTaskFinished()
{
    QFutureWatcher<void> *watcher=static_cast<QFutureWatcher<void>*>(sender());
    if(watcher!=NULL)
    {
        QString policyId=watcher->property("policy_id").toString();
        loadTemplateTimeLineControlPolicy(policyId);
        watcher->deleteLater();
    }
}

void MeshDbCaller::loadTemplateTimeLineControlPolicy(QString policyId)
{
    QFutureWatcher<TemplateTimeLineControlPolicy*> *watcher=new QFutureWatcher<TemplateTimeLineControlPolicy*>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doLoadTemplateTimeLineControlPolicyTaskFinished()));
    QFuture<TemplateTimeLineControlPolicy*> future=QtConcurrent::run(asyncLoadTemplateTimeLineControlPolicy, dbReadOnlyHolder, policyId);
    watcher->setFuture(future);
}

void MeshDbCaller::doLoadTemplateTimeLineControlPolicyTaskFinished()
{
    QFutureWatcher<TemplateTimeLineControlPolicy*> *watcher=static_cast<QFutureWatcher<TemplateTimeLineControlPolicy*>*>(sender());
    if(watcher!=NULL)
    {
        TemplateTimeLineControlPolicy *policy=watcher->result();
        emit templateTimeLineControlPolicyLoaded(policy);
        watcher->deleteLater();
    }
}

void MeshDbCaller::loadTemplateTimeLineControlItemList(QString policyId)
{
    QFutureWatcher<QList<TimeLineControlItem*>*> *watcher=new QFutureWatcher<QList<TimeLineControlItem*>*>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doLoadTemplateTimeLineControlItemListTaskFinished()));
    QFuture<QList<TimeLineControlItem*>*> future=QtConcurrent::run(asyncLoadTemplateTimeLineControlItemList, dbReadOnlyHolder, policyId);
    watcher->setFuture(future);
}

void MeshDbCaller::loadTemplateTimeLineControlItem(QString itemId)
{
    QFutureWatcher<TimeLineControlItem*> *watcher=new QFutureWatcher<TimeLineControlItem*>;
    connect(watcher, SIGNAL(finished()), this, SLOT(doLoadTemplateTimeLineControlItemTaskFinished()));
    QFuture<TimeLineControlItem*> future=QtConcurrent::run(asyncLoadTemplateTimeLineControlItem, dbReadOnlyHolder, itemId);
    watcher->setFuture(future);
}


void MeshDbCaller::doLoadTemplateTimeLineControlItemListTaskFinished()
{
    QFutureWatcher<QList<TimeLineControlItem*>*> *watcher=static_cast<QFutureWatcher<QList<TimeLineControlItem*>*>*>(sender());
    if(watcher!=NULL)
    {
        QList<TimeLineControlItem*> *itemList=watcher->result();
        emit templateTimeLineControlItemListLoaded(itemList);
        watcher->deleteLater();
    }
}

void MeshDbCaller::addTemplateTimeLineControlItem(TimeLineControlItem *item)
{
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    watcher->setProperty("policy_id", item->templatePolicyId);
    connect(watcher, SIGNAL(finished()), this, SLOT(doAddTemplateTimeLineControlItemTaskFinished()));
    item->templateItemId = Tools::getRandomUUID();
    QFuture<void> future=QtConcurrent::run(asyncAddTemplateTimeLineControlItem, dbReadWriteHolder, dbWriteMutex, item);
    watcher->setFuture(future);
}

void MeshDbCaller::doAddTemplateTimeLineControlItemTaskFinished()
{
    QFutureWatcher<void> *watcher=static_cast<QFutureWatcher<void>*>(sender());
    if(watcher!=NULL)
    {
        QString policyId=watcher->property("policy_id").toString();
        loadTemplateTimeLineControlItemList(policyId);
        watcher->deleteLater();
    }
}

void MeshDbCaller::updateTemplateTimeLineControlItem(TimeLineControlItem *item)
{
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    watcher->setProperty("item_id", item->templateItemId);
    connect(watcher, SIGNAL(finished()), this, SLOT(doUpdateTemplateTimeLineControlItemTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncUpdateTemplateTimeLineControlItem, dbReadWriteHolder, dbWriteMutex, item);
    watcher->setFuture(future);
}

void MeshDbCaller::doUpdateTemplateTimeLineControlItemTaskFinished()
{
    QFutureWatcher<void> *watcher=static_cast<QFutureWatcher<void>*>(sender());
    if(watcher!=NULL)
    {
        QString itemId=watcher->property("item_id").toString();
        loadTemplateTimeLineControlItem(itemId);
        watcher->deleteLater();
    }
}

void MeshDbCaller::doLoadTemplateTimeLineControlItemTaskFinished()
{
    QFutureWatcher<TimeLineControlItem*> *watcher=static_cast<QFutureWatcher<TimeLineControlItem*>*>(sender());
    if(watcher!=NULL)
    {
        TimeLineControlItem *item=watcher->result();
        emit templateTimeLineControlItemLoaded(item);
        watcher->deleteLater();
    }
}

void MeshDbCaller::removeTemplateTimeLineControlItem(QString policyId, QString itemId)
{
    QFutureWatcher<void> *watcher=new QFutureWatcher<void>;
    watcher->setProperty("policy_id", policyId);
    connect(watcher, SIGNAL(finished()), this, SLOT(doRemoveTemplateTimeLineControlItemTaskFinished()));
    QFuture<void> future=QtConcurrent::run(asyncRemoveTemplateTimeLineControlItem, dbReadWriteHolder, dbWriteMutex, itemId);
    watcher->setFuture(future);
}

void MeshDbCaller::doRemoveTemplateTimeLineControlItemTaskFinished()
{
    QFutureWatcher<void> *watcher=static_cast<QFutureWatcher<void>*>(sender());
    if(watcher!=NULL)
    {
        QString policyId=watcher->property("policy_id").toString();
        loadTemplateTimeLineControlItemList(policyId);
        watcher->deleteLater();
    }
}
