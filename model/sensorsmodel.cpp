#include "sensorsmodel.h"

#include "commandmanager.h"
#include <QDebug>
#include "utils/commandtaskmanager.h"
#include "domain/luxsensor.h"
#include "domain/abstractmotionsensor.h"
#include "model/meshmodel.h"
#include "model/executersmodel.h"
#include "domain/realmotionsensor.h"
#include "domain/virtualmotionsensor.h"
#include "db/meshdbmanager.h"
#include "db/meshdbcaller.h"
#include "domain/co1sensor.h"
#include "domain/co2sensor.h"
#include "domain/pmsensor.h"
#include "domain/realswitchsensor.h"
#include "domain/realswitchsensor2ch.h"
#include "domain/virtualswitchsensor.h"
#include "model/nodegroupsmodel.h"
#include "utils/meshsavingbox.h"
#include "domain/realmotionandluxcamerasensor.h"
#include "objectfactory.h"
#include "parsermanager.h"
#include "parser/sensorparser.h"
#include "utils/meshlogmanager.h"

SensorsModel::SensorsModel(MeshModel *meshModel, QObject *parent)
    : QObject(parent),
      m_meshModel(meshModel)
{
    m_realSensorTypeList.push_back(Sensor::LuxSensor);
    m_realSensorTypeList.push_back(Sensor::RealMotionSensor);
    m_realSensorTypeList.push_back(Sensor::PMSensor);
    m_realSensorTypeList.push_back(Sensor::CO1Sensor);
    m_realSensorTypeList.push_back(Sensor::CO2Sensor);
    m_realSensorTypeList.push_back(Sensor::RealSwitchSensor);
    m_realSensorTypeList.push_back(Sensor::RealSwitchSensor2CH);
    m_realSensorTypeList.push_back(Sensor::RealMotionAndLuxCameraSensor);
    m_realSensorTypeList.push_back(Sensor::RefrgTemperatureHumiditySensor);
    m_realSensorTypeList.push_back(Sensor::UVLightSensor);
    m_realSensorTypeList.push_back(Sensor::HandWashingSensor);

    m_virtualSensorTypeList.push_back(Sensor::VirtualMotionSensor);
    m_virtualSensorTypeList.push_back(Sensor::VirtualSwitchSensor);

    m_checkOnlineStatusChangeTimer.setInterval(2000);
    connect(&m_checkOnlineStatusChangeTimer, SIGNAL(timeout()),
            this, SLOT(doCheckOnlineStatusChange()));
    m_checkOnlineStatusChangeTimer.start();
}


void SensorsModel::updateSensorByDBReason(Sensor *dbSensor)
{
    SensorParser *sensorParser=ParserManager::instance()->getSensorParserByType(dbSensor->sensorType());
    if(sensorParser==NULL)
    {
        return;
    }
    Sensor *oldSensor=sensorMap.value(dbSensor->sensorId());
    if(oldSensor==NULL)
    {
        oldSensor=ObjectFactory::instance()->createSensorByTypeText(dbSensor->typeText());
        if(oldSensor==NULL)
        {
            return;
        }
        oldSensor->setSensorId(dbSensor->sensorId());
        sensorParser->updateSensorFromDB(m_meshModel, dbSensor, oldSensor);
        sensorMap.insert(oldSensor->sensorId(), oldSensor);
        sensorList.push_back(oldSensor);
    }
    else
    {
        sensorParser->updateSensorFromDB(m_meshModel, dbSensor, oldSensor);
    }

    //用来完成加载虚拟感应传感器的绑定ID数据到SensorsModel中等功能
    sensorParser->updateReferredSensorDataAndActionAfterDBUpdate(m_meshModel, oldSensor);
}

void SensorsModel::updateSensorByOnlineStatusReason(Sensor *sensor)
{
    SensorParser *sensorParser=ParserManager::instance()->getSensorParserByTypeText(sensor->typeText());
    if(sensorParser==NULL)
    {
        return;
    }
    if(sensorMap.contains(sensor->sensorId()))
    {
        Sensor *oldSensor=sensorMap.value(sensor->sensorId());
        sensorParser->updateSensorFromOnlineStatus(m_meshModel, sensor, oldSensor);
        emit dataChanged();
    }
    else
    {
        Sensor *copySensor=ObjectFactory::instance()->createSensorByTypeText(sensor->typeText());
        if(copySensor==NULL)
        {
            return;
        }
        copySensor->setSensorId(sensor->sensorId());
        copySensor->setName(copySensor->sensorId());
        sensorParser->updateSensorFromOnlineStatus(m_meshModel, sensor, copySensor);
        sensorList.push_back(copySensor);
        sensorMap.insert(copySensor->sensorId(), copySensor);
//        emit modelChanged();
        updateSensorNumber(false, sensor);

        QMap<QString, QPair<QString, QString> > *nameMap=new QMap<QString, QPair<QString, QString> >;
        nameMap->insert(copySensor->sensorId(),
                        QPair<QString, QString>(copySensor->typeText(), copySensor->getName()));
        m_meshModel->dbManager()->dbCaller()->setSensorNodesName(nameMap);
    }

    // 获取到传感器OnlineStatus数据时，因为不带有传感器实际数据，只有节点是否在线等与传感器数据无关的信息，所以此处不需要触发上报动作
}

void SensorsModel::updateSensorByUserDataNotifyReason(Sensor *sensor)
{
    SensorParser *sensorParser=ParserManager::instance()->getSensorParserByType(sensor->sensorType());
    if(sensorParser==NULL)
    {
        return;
    }
    if(sensorMap.contains(sensor->sensorId()))
    {
        Sensor *oldSensor=sensorMap.value(sensor->sensorId());
        sensorParser->updateSensorFromUserDataNotify(m_meshModel, sensor, oldSensor);
        emit dataChanged();
    }
    else
    {
        Sensor *copySensor=ObjectFactory::instance()->createSensorByTypeText(sensor->typeText());
        if(copySensor==NULL)
        {
            return;
        }
        copySensor->setSensorId(sensor->sensorId());
        copySensor->setName(copySensor->sensorId());
        sensorParser->updateSensorFromUserDataNotify(m_meshModel, sensor, copySensor);
        sensorList.push_back(copySensor);
        sensorMap.insert(copySensor->sensorId(), copySensor);
//        emit modelChanged();
        updateSensorNumber(false, sensor);

        QMap<QString, QPair<QString, QString> > *nameMap=new QMap<QString, QPair<QString, QString> >;
        nameMap->insert(copySensor->sensorId(),
                        QPair<QString, QString>(copySensor->typeText(), copySensor->getName()));
        m_meshModel->dbManager()->dbCaller()->setSensorNodesName(nameMap);
    }

    // 此处是传感器上报了实际传感数据，所以需要触发上报动作
    Sensor *reportSensor=sensorMap.value(sensor->sensorId());
    if(reportSensor==NULL)
    {
        return;
    }
}

void SensorsModel::updateReferredSensorDataAndActionAfterUserDataNotifyUpdate(Sensor *pSensor)
{
    SensorParser *sensorParser=ParserManager::instance()->getSensorParserByType(pSensor->sensorType());
    if(sensorParser)
    {
        Sensor *sensor=getSensorBySensorId(pSensor->sensorId());
        if(sensor)
        {
            sensorParser->updateReferredSensorDataAndActionAfterUserDataNotifyUpdate(m_meshModel, sensor);
        }
    }
}

void SensorsModel::updateSensorNumber(bool updateAll, Sensor *causeSensor)
{
    if(updateAll)
    {
        m_sensorNumberMap.clear();
        QList<Sensor*> tmpSensorList=this->sensorMap.values();
        for(int i=0; i<tmpSensorList.size(); i++)
        {
            Sensor *tmpSensor=tmpSensorList.at(i);
            m_sensorNumberMap.insert(tmpSensor->sensorType(), m_sensorNumberMap.value(tmpSensor->sensorType(), 0)+1);
        }
        emit updateAllSensorNumberDisplay();
    }
    else
    {
        if(causeSensor==NULL)
        {
            return;
        }
        m_sensorNumberMap.insert(causeSensor->sensorType(), 0);
        QList<Sensor*> tmpSensorList=this->sensorMap.values();
        for(int i=0; i<tmpSensorList.size(); i++)
        {
            Sensor *tmpSensor=tmpSensorList.at(i);
            if(tmpSensor->sensorType()!=causeSensor->sensorType())
            {
                continue;
            }
            m_sensorNumberMap.insert(tmpSensor->sensorType(), m_sensorNumberMap.value(tmpSensor->sensorType(), 0)+1);
        }
        emit updateAllSensorNumberDisplay();
    }
}

void SensorsModel::updateRealMotionSensorToVirtualMotionSensorMap(const QString &virtualSensorId, const QList<QString> &realSensorIdList)
{
    QList<QSet<QString>*> virtualMotionSensorIdSetList=realMotionSensorIdToVirtualMotionSensorIdMap.values();
    for(int i=0; i<virtualMotionSensorIdSetList.size(); i++)
    {
        QSet<QString> *virtualMotionSensorIdSet=virtualMotionSensorIdSetList.at(i);
        virtualMotionSensorIdSet->remove(virtualSensorId);
    }

    for(int i=0; i<realSensorIdList.size(); i++)
    {
        QString realSensorId=realSensorIdList.at(i);
        QSet<QString> *set=realMotionSensorIdToVirtualMotionSensorIdMap.value(realSensorId);
        if(set==NULL)
        {
            set=new QSet<QString>;
            realMotionSensorIdToVirtualMotionSensorIdMap.insert(realSensorId, set);
        }
        set->insert(virtualSensorId);
    }
}

void SensorsModel::restartTimerWhenMotionSensorTriggered(const QString &sensorId)
{
    QTimer *timer=motionCheckedHoldingTimerMap.value(sensorId);
    if(timer==NULL)
    {
        timer=new QTimer;
        timer->setInterval(5000);
        timer->setSingleShot(true);
        timer->setProperty("motion_sensor_id", sensorId);
        connect(timer, SIGNAL(timeout()), this, SLOT(doMotionCheckedHoldingTimerTimeout()));
        motionCheckedHoldingTimerMap.insert(sensorId, timer);
    }
    timer->stop();
    timer->start();
}

void SensorsModel::onRealSwitchSensorTrig(const QString &realSwitchSensorId, quint8 realSwitchSensorBindGroupId, int trigValue)
{
    QList<VirtualSwitchSensor*> vList=this->getBindingVirtualSwitchSensorListByRealSwitchSensorId(realSwitchSensorId);
    for(int i=0; i<vList.size(); i++)
    {
        VirtualSwitchSensor *virtualSwitchSensor=vList.at(i);
        virtualSwitchSensor->setCurrentTrigValue(trigValue);
        QList<quint8> logicalGroupIDList=virtualSwitchSensor->getLogicGroupIDList();
        switch (virtualSwitchSensor->currentTrigControlType()) {
            case RealSwitchSensor::SwitchNone:
                break;
            case RealSwitchSensor::SwitchOnDutySchedule:
            {
                m_meshModel->nodeGroupsModel()->setGroupScheduleType(realSwitchSensorBindGroupId, NodeGroup::ScheduleByOnDuty);
                for(int i=0; i<logicalGroupIDList.size(); i++)
                {
                    quint8 logicGroupID=logicalGroupIDList.at(i);
                    if(realSwitchSensorBindGroupId==logicGroupID)
                    {
                        continue;
                    }
                    m_meshModel->nodeGroupsModel()->setGroupScheduleType(logicGroupID, NodeGroup::ScheduleByOnDuty);
                }
            }
            break;
            case RealSwitchSensor::SwitchOffDutySchedule:
            {
                m_meshModel->nodeGroupsModel()->setGroupScheduleType(realSwitchSensorBindGroupId, NodeGroup::ScheduleByOffDuty);
                for(int i=0; i<logicalGroupIDList.size(); i++)
                {
                    quint8 logicGroupID=logicalGroupIDList.at(i);
                    if(realSwitchSensorBindGroupId==logicGroupID)
                    {
                        continue;
                    }
                    m_meshModel->nodeGroupsModel()->setGroupScheduleType(logicGroupID, NodeGroup::ScheduleByOffDuty);
                }
            }
            break;
            case RealSwitchSensor::SwitchDateSchedule:
            {
                m_meshModel->nodeGroupsModel()->setGroupScheduleType(realSwitchSensorBindGroupId, NodeGroup::ScheduleByDate);
                for(int i=0; i<logicalGroupIDList.size(); i++)
                {
                    quint8 logicGroupID=logicalGroupIDList.at(i);
                    if(realSwitchSensorBindGroupId==logicGroupID)
                    {
                        continue;
                    }
                    m_meshModel->nodeGroupsModel()->setGroupScheduleType(logicGroupID, NodeGroup::ScheduleByDate);
                }
            }
            break;
            default:
            {
                m_meshModel->nodeGroupsModel()->setGroupScheduleType(realSwitchSensorBindGroupId, NodeGroup::ScheduleOff);
                for(int i=0; i<logicalGroupIDList.size(); i++)
                {
                    quint8 logicGroupID=logicalGroupIDList.at(i);
                    if(realSwitchSensorBindGroupId==logicGroupID)
                    {
                        continue;
                    }
                    m_meshModel->nodeGroupsModel()->setGroupScheduleType(logicGroupID, NodeGroup::ScheduleOff);
                }
            }
            break;
        }
    }

    emit dataChanged();
}

void SensorsModel::executeRealMotionSensorSpecialLogicWhenUpdateReferredData(RealMotionSensor *realMotionSensor)
{
    restartTimerWhenMotionSensorTriggered(realMotionSensor->sensorId());
    emit motionSensorNotify(realMotionSensor->sensorId());
    QSet<QString> *virtualSensorIdSet=realMotionSensorIdToVirtualMotionSensorIdMap.value(realMotionSensor->sensorId());
    if(virtualSensorIdSet!=NULL)
    {
        bool modelDataChanged=false;
        QList<QString> virtualSensorIdList=virtualSensorIdSet->toList();
        for(int i=0; i<virtualSensorIdList.size(); i++)
        {
            Sensor *sensor=this->sensorMap.value(virtualSensorIdList.at(i));
            if(sensor!=NULL && sensor->sensorType()==Sensor::VirtualMotionSensor)
            {
                VirtualMotionSensor *virtualMotionSensor=static_cast<VirtualMotionSensor*>(sensor);
                if(virtualMotionSensor!=NULL)
                {
                    virtualMotionSensor->setChecked(true);
                    restartTimerWhenMotionSensorTriggered(virtualMotionSensor->sensorId());
                }
                updateSensorByUserDataNotifyReason(virtualMotionSensor);
                emit motionSensorNotify(sensor->sensorId());
                modelDataChanged=true;
            }
        }
        if(modelDataChanged)
        {
            emit dataChanged();
        }
    }
}

void SensorsModel::executeRealLuxSensorSpecialLogicWhenUpdateReferredData(LuxSensor *luxSensor)
{
    emit luxSensorNotify(luxSensor->sensorId(), luxSensor->getLux());
}

void SensorsModel::executeRealSwitchSensorSpecialLogicWhenUpdateReferredData(RealSwitchSensor *realSwitchSensor)
{
    onRealSwitchSensorTrig(realSwitchSensor->sensorId(), realSwitchSensor->bindGroupId(), realSwitchSensor->trigValue());
}

void SensorsModel::executeRealSwitchSensorSpecialLogicWhenUpdateReferredData(RealSwitchSensor2CH *realSwitchSensor)
{
    onRealSwitchSensorTrig(realSwitchSensor->sensorId(), realSwitchSensor->bindGroupId(), realSwitchSensor->trigValue());
}

void SensorsModel::executeRealMotionLuxCameraSpecialLogicWhenUpdateReferredData(RealMotionAndLuxCameraSensor *realMotionLuxCamera)
{
    for(int channel=1; channel<=4; channel++)
    {
        LuxSensor *luxSensor=new LuxSensor;
        luxSensor->setBluetoothAddress(realMotionLuxCamera->getBluetoothAddress());
        luxSensor->setSensorId(QString("%1.%2.%3").arg(realMotionLuxCamera->sensorId()).arg(LUX_SENSOR_SHORT_TYPE_TEXT).arg(channel));
        luxSensor->setLux(realMotionLuxCamera->luxChannelValue(channel)*100);
        updateSensorByUserDataNotifyReason(luxSensor);
        updateReferredSensorDataAndActionAfterUserDataNotifyUpdate(luxSensor);
        delete luxSensor;

        RealMotionSensor *realMotionSensor=new RealMotionSensor;
        realMotionSensor->setChecked(realMotionLuxCamera->motionChannelTrigged(channel));
        realMotionSensor->setBluetoothAddress(realMotionLuxCamera->getBluetoothAddress());
        realMotionSensor->setSensorId(QString("%1.%2.%3").arg(realMotionLuxCamera->sensorId()).arg(REAL_MOTION_SENSOR_SHORT_TYPE_TEXT).arg(channel));
        updateSensorByUserDataNotifyReason(realMotionSensor);
        updateReferredSensorDataAndActionAfterUserDataNotifyUpdate(realMotionSensor);
        delete realMotionSensor;
    }
}

QList<Sensor::SensorType> SensorsModel::allNumberableSensorTypeList()
{
    QList<Sensor::SensorType> sensorTypeList=m_sensorNumberMap.keys();
    return sensorTypeList;
}

int SensorsModel::count()
{
    return sensorList.size();
}

Sensor *SensorsModel::at(int index)
{
    if(index<sensorList.size())
    {
        return sensorList.at(index);
    }
    else
    {
        return NULL;
    }
}

Sensor *SensorsModel::getSensorBySensorId(const QString &sensorId)
{
    return sensorMap.value(sensorId);
}

void SensorsModel::setGroupList(quint8 addr, QList<quint8> groupList)
{
    for(int i=0; i<sensorList.size(); i++)
    {
        Sensor *sensor=sensorList.at(i);
        if(sensor->isConfigurable())
        {
            continue;
        }
        if(sensor->getBluetoothAddress()==addr)
        {
            sensor->setGroupList(groupList);
        }
    }
    emit modelChanged();
}

QString SensorsModel::getNodeName(const QString &sensorId)
{
    Sensor *sensor=sensorMap.value(sensorId);
    if(sensor==NULL)
    {
        return "";
    }
    else
    {
        return sensor->getName();
    }
}

int SensorsModel::getLuxValue(const QString &sensorId)
{
    Sensor *sensor=sensorMap.value(sensorId);
    if(sensor==NULL)
    {
        return -1;
    }
    else
    {
        if(sensor->sensorType()==Sensor::LuxSensor)
        {
            LuxSensor *luxSensor=static_cast<LuxSensor*>(sensor);
            if(luxSensor==NULL)
            {
                return -1;
            }
            else
            {
                return luxSensor->getLux();
            }
        }
        else
        {
            return -1;
        }
    }
}

bool SensorsModel::contains(const QString sensorId)
{
    return sensorMap.contains(sensorId);
}

QList<Sensor *> SensorsModel::getAll()
{
    return this->sensorList;
}

QList<Sensor *> SensorsModel::allRealMotionSensor()
{
    QList<Sensor *> virtualList;
    int len=this->sensorList.size();
    for(int i=0; i<len; i++)
    {
        Sensor *sensor=sensorList.at(i);
        if(sensor->sensorType()==Sensor::RealMotionSensor)
        {
            virtualList.push_back(sensor);
        }
    }
    return virtualList;
}

void SensorsModel::addVirtualSensorToDB(Sensor *sensor)
{
    SensorParser *sensorParser=ParserManager::instance()->getSensorParserByType(sensor->sensorType());
    if(sensorParser)
    {
        QString id;
        for(int i=0; i<500; i++)
        {
            QString tmpId=QString("%1.%2").arg(sensor->shortTypeText()).arg(i);
            if(!this->contains(tmpId))
            {
                id=tmpId;
                break;
            }
        }
        if(!id.isEmpty())
        {
            sensor->setSensorId(id);
            Sensor *copySensor=ObjectFactory::instance()->createSensorByTypeText(sensor->typeText());
            if(copySensor)
            {
                copySensor->setSensorId(sensor->sensorId());
                sensorParser->updateSensorFromDB(m_meshModel, sensor, copySensor);
                m_meshModel->dbManager()->dbCaller()->addSensor(copySensor);
            }
        }
    }
}

void SensorsModel::updateVirtualSensorToDB(Sensor *sensor)
{
    SensorParser *sensorParser=ParserManager::instance()->getSensorParserByTypeText(sensor->typeText());
    if(sensorParser==NULL)
    {
        return;
    }
    Sensor *copySensor=ObjectFactory::instance()->createSensorByTypeText(sensor->typeText());
    if(copySensor==NULL)
    {
        return;
    }
    copySensor->setSensorId(sensor->sensorId());
    sensorParser->updateSensorFromDB(m_meshModel, sensor, copySensor);
    m_meshModel->dbManager()->dbCaller()->updateSensor(copySensor);
}

void SensorsModel::deleteVirtualSensor(const QString &sensorId)
{
    m_meshModel->dbManager()->dbCaller()->removeSensor(sensorId);
}

void SensorsModel::clear()
{
    QList<Sensor*> notDeleteSensorList;

    while(!sensorList.isEmpty())
    {
        Sensor *sensor=sensorList.takeFirst();
        if(sensor->isVirtual())
        {
            notDeleteSensorList.push_back(sensor);
        }
        else
        {
            delete sensor;
        }
    }
    sensorList.clear();
    sensorMap.clear();

    sensorList.append(notDeleteSensorList);
    for(int i=0; i<notDeleteSensorList.size(); i++)
    {
        Sensor *sensor=notDeleteSensorList.at(i);
        this->sensorMap.insert(sensor->sensorId(), sensor);
    }
    emit modelChanged();
    updateSensorNumber();
}

QList<VirtualSwitchSensor *> SensorsModel::getBindingVirtualSwitchSensorListByRealSwitchSensorId(QString realSwitchSensorId)
{
    QList<VirtualSwitchSensor*> vSSensorList;
    for(int i=0; i<sensorList.size(); i++)
    {
        Sensor *sensor=sensorList.at(i);
        if(sensor->sensorType()!=Sensor::VirtualSwitchSensor)
        {
            continue;
        }
        VirtualSwitchSensor *vSSensor=static_cast<VirtualSwitchSensor*>(sensor);
        if(vSSensor==NULL)
        {
            continue;
        }
        if(vSSensor->getBindingSwitchSensorId()!=realSwitchSensorId)
        {
            continue;
        }
        vSSensorList.push_back(vSSensor);
    }
    return vSSensorList;
}

bool SensorsModel::parseADataFrame(const QByteArray &dataFrame)
{
    bool parsed=true;
    do{
        if(dataFrame.size()<20)
        {
            parsed=false;
            break;
        }
        if(dataFrame.at(7)==CMD_MARK_CODE_LIGHT_STATUS)
        {
            if(dataFrame.at(10)!=0)  //第一路
            {
                quint8 nodeTypeCode=dataFrame.at(13);
                qDebug() << "SensorsModel::parseADataFrame, dataFrame=" <<
                                        QString::fromLatin1(dataFrame.toHex()) <<
                            "Light Status, CH1: nodeTypeCode=" << hex << nodeTypeCode;
                if(ObjectFactory::instance()->isSensorNodeTypeCode(nodeTypeCode))
                {
                    Sensor *sensor=ObjectFactory::instance()->createSensorByNodeTypeCode(nodeTypeCode);
                    if(sensor!=NULL)
                    {
                        SensorParser *sensorParser=ParserManager::instance()->getSensorParserByTypeText(sensor->typeText());
                        if(sensorParser!=NULL)
                        {
                            sensorParser->parseOnlineStatus4BytesDataFrameToSensor(dataFrame.mid(10, 4), sensor);
                            updateSensorByOnlineStatusReason(sensor);
                        }
                        delete sensor;
                    }
                }
                else
                {
                    parsed=false;
                }
            }
            if(dataFrame.at(14)!=0)  //第二路
            {
                quint8 nodeTypeCode=dataFrame.at(17);
                qDebug() << "SensorsModel::parseADataFrame, dataFrame=" <<
                                        QString::fromLatin1(dataFrame.toHex()) <<
                            "Light Status, CH2: nodeTypeCode=" << hex << nodeTypeCode;
                if(ObjectFactory::instance()->isSensorNodeTypeCode(nodeTypeCode))
                {
                    Sensor *sensor=ObjectFactory::instance()->createSensorByNodeTypeCode(nodeTypeCode);
                    if(sensor!=NULL)
                    {
                        SensorParser *sensorParser=ParserManager::instance()->getSensorParserByTypeText(sensor->typeText());
                        if(sensorParser!=NULL)
                        {
                            sensorParser->parseOnlineStatus4BytesDataFrameToSensor(dataFrame.mid(14, 4), sensor);
                            updateSensorByOnlineStatusReason(sensor);
                        }
                        delete sensor;
                    }
                }
                else
                {
                    parsed=false;
                }
            }
        }
        else if(dataFrame.at(7)==CMD_MARK_CODE_USER_NOTIFY_DATA)
        {
            quint8 code=dataFrame.at(10);
            qDebug() << "SensorsModel::parseADataFrame, dataFrame=" <<
                                QString::fromLatin1(dataFrame.toHex()) <<
                        "User Notify: Code=" << hex << code;
            if(ObjectFactory::instance()->isSensorUserDataTypeCode(code))
            {
                Sensor *sensor=ObjectFactory::instance()->createSensorByNotifyDataCode(code);
                if(sensor!=NULL)
                {
                    SensorParser *sensorParser=ParserManager::instance()->getSensorParserBySensorCode(code);
                    if(sensorParser!=NULL)
                    {
                        sensorParser->parseUserDataFrameToSensor(dataFrame, sensor);
                        updateSensorByUserDataNotifyReason(sensor);
                        updateReferredSensorDataAndActionAfterUserDataNotifyUpdate(sensor);
                    }
                    delete sensor;
                }
            }
            else
            {
                parsed=false;
            }
        }
        else if(dataFrame.at(7)==CMD_MARK_CODE_GROUP_G8_NOTIFY_DATA)
        {
            quint8 addr=(quint8)dataFrame.at(3);
            QList<quint8> groupList;
            for(int i=10; i<18; i++)
            {
                quint8 group=dataFrame.at(i);
                if(group!=(quint8)0xFF)
                {
                    groupList.append(group);
                }
            }
            setGroupList(addr, groupList);
        }
    }while(false);

    return parsed;
}

int SensorsModel::sensorNumber(Sensor::SensorType sensorType)
{
    return m_sensorNumberMap.value(sensorType, 0);
}

QList<Sensor::SensorType> SensorsModel::realSensorTypeList()
{
    return this->m_realSensorTypeList;
}



void SensorsModel::doMotionCheckedHoldingTimerTimeout()
{
    QString motionSensorId=sender()->property("motion_sensor_id").toString();
    Sensor *sensor=sensorMap.value(motionSensorId);
    if(sensor!=NULL)
    {
        if(sensor->sensorType()==Sensor::RealMotionSensor)
        {
            RealMotionSensor *motionSensor=static_cast<RealMotionSensor*>(sensor);
            if(motionSensor!=NULL)
            {
                motionSensor->setChecked(false);
            }
        }
        else if(sensor->sensorType()==Sensor::VirtualMotionSensor)
        {
            VirtualMotionSensor *virtualMotionSensor=static_cast<VirtualMotionSensor*>(sensor);
            if(virtualMotionSensor!=NULL)
            {
                virtualMotionSensor->setChecked(false);
            }
        }
    }
    emit dataChanged();
}

void SensorsModel::doCheckOnlineStatusChange()
{
    int len=sensorList.size();
    bool changed=false;
    for(int i=0; i<len; i++)
    {
        Sensor *sensor=sensorList.at(i);
        bool oldOnline=sensor->isOnline();
        sensor->checkOnlineStatusTimeoutChange(60*1000);  //60 seconds
        if(oldOnline!=sensor->isOnline())
        {
            changed=true;
        }
    }
    if(changed)
    {
        emit modelChanged();
    }
}

void SensorsModel::doSyncSensorNodes(QMap<QString, Sensor*> *sensorMap)
{
    m_sensorNumberMap.clear();
    QList<Sensor*> sensorList=sensorMap->values();
    QList<QString> removeKeys=this->sensorMap.keys();

    while(!sensorList.isEmpty())
    {
        Sensor *sensor=sensorList.takeFirst();
        removeKeys.removeAll(sensor->sensorId());
        updateSensorByDBReason(sensor);
        delete sensor;
    }

    for(int i=0; i<removeKeys.size(); i++)
    {
        QString removeKey=removeKeys.at(i);
        Sensor *sensor=this->sensorMap.value(removeKey);
        if(sensor==NULL)
        {
            continue;
        }
        if(sensor->isVirtual() || (!sensor->isTouched()))
        {
            this->sensorList.removeAll(sensor);
            this->sensorMap.remove(removeKey);
            if(sensor->sensorType()==Sensor::VirtualMotionSensor)
            {
                updateRealMotionSensorToVirtualMotionSensorMap(sensor->sensorId(), QList<QString>());
            }
            delete sensor;
        }
    }

    emit modelChanged();
    delete sensorMap;
    updateSensorNumber();
}
