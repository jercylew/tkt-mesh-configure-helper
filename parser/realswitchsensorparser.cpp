#include "realswitchsensorparser.h"
#include "domain/realswitchsensor.h"
#include "model/meshmodel.h"
#include "model/sensorsmodel.h"
#include "widget/sensorframe.h"

RealSwitchSensorParser::RealSwitchSensorParser()
{

}

void RealSwitchSensorParser::parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor)
{
    SensorParser::parseUserDataFrameToSensor(dataFrame, sensor);
    RealSwitchSensor *realSwitchSensor=static_cast<RealSwitchSensor*>(sensor);
    if(realSwitchSensor)
    {
        realSwitchSensor->setTrigValue(dataFrame.at(12)&0xFF);
        realSwitchSensor->setBindGroupId(dataFrame.at(14));
    }
}

void RealSwitchSensorParser::updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor)
{
    if(oldSensor==NULL)
    {
        return;
    }
    SensorParser::updateSensorFromUserDataNotify(meshModel, newSensor, oldSensor);
    RealSwitchSensor *oldSwitchSensor=static_cast<RealSwitchSensor*>(oldSensor);
    RealSwitchSensor *newSwitchSensor=static_cast<RealSwitchSensor*>(newSensor);
    oldSwitchSensor->setTrigValue(newSwitchSensor->trigValue());
    oldSwitchSensor->setBindGroupId(newSwitchSensor->bindGroupId());
}

void RealSwitchSensorParser::updateReferredSensorDataAndActionAfterUserDataNotifyUpdate(MeshModel *meshModel, Sensor *sensor)
{
    RealSwitchSensor *realSwitchSensor=static_cast<RealSwitchSensor*>(sensor);
    if(realSwitchSensor)
    {
        meshModel->sensorsModel()->executeRealSwitchSensorSpecialLogicWhenUpdateReferredData(realSwitchSensor);
    }
}

void RealSwitchSensorParser::parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame)
{
    SensorParser::parseSensorToSensorFrame(sensor, sensorFrame);
    RealSwitchSensor *switchSensor=static_cast<RealSwitchSensor*>(sensor);
    if(switchSensor)
    {
        sensorFrame->setStateText(QString("G[%1]:%2").arg(switchSensor->bindGroupId()).arg(switchSensor->trigValue()));
        sensorFrame->getIconLabel()->setProperty("type", "real_switch_sensor");
    }
}
