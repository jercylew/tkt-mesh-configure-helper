#include "realswitchsensorparser2ch.h"
#include "domain/realswitchsensor2ch.h"
#include "model/meshmodel.h"
#include "model/sensorsmodel.h"
#include "widget/sensorframe.h"

RealSwitchSensorParser2CH::RealSwitchSensorParser2CH()
{

}

void RealSwitchSensorParser2CH::parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor)
{
    SensorParser::parseUserDataFrameToSensor(dataFrame, sensor);
    RealSwitchSensor2CH *realSwitchSensor=static_cast<RealSwitchSensor2CH*>(sensor);
    if(realSwitchSensor)
    {
        realSwitchSensor->setTrigValue(dataFrame.at(12)&0xFF);
        realSwitchSensor->setBindGroupId(dataFrame.at(14));
    }
}

void RealSwitchSensorParser2CH::updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor)
{
    if(oldSensor==NULL)
    {
        return;
    }
    SensorParser::updateSensorFromUserDataNotify(meshModel, newSensor, oldSensor);
    RealSwitchSensor2CH *oldSwitchSensor=static_cast<RealSwitchSensor2CH*>(oldSensor);
    RealSwitchSensor2CH *newSwitchSensor=static_cast<RealSwitchSensor2CH*>(newSensor);
    oldSwitchSensor->setTrigValue(newSwitchSensor->trigValue());
    oldSwitchSensor->setBindGroupId(newSwitchSensor->bindGroupId());
}

void RealSwitchSensorParser2CH::updateReferredSensorDataAndActionAfterUserDataNotifyUpdate(MeshModel *meshModel, Sensor *sensor)
{
    RealSwitchSensor2CH *realSwitchSensor=static_cast<RealSwitchSensor2CH*>(sensor);
    if(realSwitchSensor)
    {
        meshModel->sensorsModel()->executeRealSwitchSensorSpecialLogicWhenUpdateReferredData(realSwitchSensor);
    }
}

void RealSwitchSensorParser2CH::parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame)
{
    SensorParser::parseSensorToSensorFrame(sensor, sensorFrame);
    RealSwitchSensor2CH *switchSensor=static_cast<RealSwitchSensor2CH*>(sensor);
    if(switchSensor)
    {
        sensorFrame->setStateText(QString("G[%1]:%2").arg(switchSensor->bindGroupId()).arg(switchSensor->trigValue()));
        sensorFrame->getIconLabel()->setProperty("type", "real_switch_sensor_2ch");
    }
}

