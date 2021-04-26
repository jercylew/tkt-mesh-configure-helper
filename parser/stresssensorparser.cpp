#include "stresssensorparser.h"
#include "domain/stresssensor.h"
#include "model/meshmodel.h"
#include "widget/sensorframe.h"

StressSensorParser::StressSensorParser()
{

}

void StressSensorParser::parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor)
{
    SensorParser::parseUserDataFrameToSensor(dataFrame, sensor);
    StressSensor *stressSensor=static_cast<StressSensor*>(sensor);
    if(stressSensor)
    {
        stressSensor->setStress((quint16)(((quint8)dataFrame.at(12))*256+(quint8)dataFrame.at(13)));
        stressSensor->setTriggered(dataFrame.at(14)!=0);
        stressSensor->setDeltaStress((quint16)(((quint8)dataFrame.at(15))*256+(quint8)dataFrame.at(16)));
        stressSensor->setDeltaStressTriggered(dataFrame.at(17)!=0);
    }
}

void StressSensorParser::updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor)
{
    if(oldSensor==NULL)
    {
        return;
    }
    SensorParser::updateSensorFromUserDataNotify(meshModel, newSensor, oldSensor);
    StressSensor *oldStressSensor=static_cast<StressSensor *>(oldSensor);
    StressSensor *newStressSensor=static_cast<StressSensor *>(newSensor);
    oldStressSensor->setStress(newStressSensor->stress());
    oldStressSensor->setTriggered(newStressSensor->triggered());
    oldStressSensor->setDeltaStress(newStressSensor->deltaStress());
    oldStressSensor->setDeltaStressTriggered(newStressSensor->deltaStressTriggered());
}

void StressSensorParser::parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame)
{
    SensorParser::parseSensorToSensorFrame(sensor, sensorFrame);
    StressSensor *stressSensor=static_cast<StressSensor*>(sensor);
    if(stressSensor)
    {
        QString stateText=QString("CH1: %1, CH2: %2").arg(stressSensor->triggered()?1:0).arg(stressSensor->deltaStressTriggered()?1:0);
        sensorFrame->setStateText(stateText);
        sensorFrame->getIconLabel()->setProperty("type", "stress_sensor");
    }
}
