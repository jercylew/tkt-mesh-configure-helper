#include "realmotionsensorparser.h"
#include "domain/realmotionsensor.h"
#include <QObject>
#include "model/meshmodel.h"
#include "model/sensorsmodel.h"
#include "widget/sensorframe.h"

RealMotionSensorParser::RealMotionSensorParser()
{

}

void RealMotionSensorParser::parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor)
{
    SensorParser::parseUserDataFrameToSensor(dataFrame, sensor);
    RealMotionSensor *realMotionSensor=static_cast<RealMotionSensor*>(sensor);
    if(realMotionSensor)
    {
        realMotionSensor->setChecked(true);
    }
}

void RealMotionSensorParser::updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor)
{
    if(oldSensor==NULL)
    {
        return;
    }
    SensorParser::updateSensorFromUserDataNotify(meshModel, newSensor, oldSensor);
    RealMotionSensor *oldMotionSensor=static_cast<RealMotionSensor*>(oldSensor);
    RealMotionSensor *newMotionSensor=static_cast<RealMotionSensor*>(newSensor);
    oldMotionSensor->setChecked(newMotionSensor->isChecked());
}

void RealMotionSensorParser::updateReferredSensorDataAndActionAfterUserDataNotifyUpdate(MeshModel *meshModel, Sensor *sensor)
{
    RealMotionSensor *realMotionSensor=static_cast<RealMotionSensor*>(sensor);
    if(realMotionSensor)
    {
        if(realMotionSensor->isChecked())
        {
            meshModel->sensorsModel()->executeRealMotionSensorSpecialLogicWhenUpdateReferredData(realMotionSensor);
        }
    }
}

void RealMotionSensorParser::parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame)
{
    SensorParser::parseSensorToSensorFrame(sensor, sensorFrame);
    RealMotionSensor *realMotionSensor=static_cast<RealMotionSensor*>(sensor);
    if(realMotionSensor)
    {
        sensorFrame->setStateText(realMotionSensor->isChecked()?QObject::tr("SensorListViewModel.ManChecked"):QObject::tr("SensorListViewModel.ManNotChecked"));
        sensorFrame->getIconLabel()->setProperty("type", "sensor_motion");
    }
}
