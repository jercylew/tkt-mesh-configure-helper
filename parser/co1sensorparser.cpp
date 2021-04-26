#include "co1sensorparser.h"
#include "domain/co1sensor.h"
#include "widget/sensorframe.h"

CO1SensorParser::CO1SensorParser()
{

}

void CO1SensorParser::parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor)
{
    SensorParser::parseUserDataFrameToSensor(dataFrame, sensor);
    CO1Sensor *co1Sensor=static_cast<CO1Sensor*>(sensor);
    if(co1Sensor)
    {
        co1Sensor->setCo1((quint16)(((quint8)dataFrame.at(12))*256+(quint8)dataFrame.at(13)));
    }
}

void CO1SensorParser::updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor)
{
    if(oldSensor==NULL)
    {
        return;
    }
    SensorParser::updateSensorFromUserDataNotify(meshModel, newSensor, oldSensor);
    CO1Sensor *oldCO1Sensor=static_cast<CO1Sensor*>(oldSensor);
    CO1Sensor *newCO1Sensor=static_cast<CO1Sensor*>(newSensor);
    oldCO1Sensor->setCo1(newCO1Sensor->getCo1());
}

void CO1SensorParser::parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame)
{
    SensorParser::parseSensorToSensorFrame(sensor, sensorFrame);
    CO1Sensor *co1Sensor=static_cast<CO1Sensor*>(sensor);
    if(co1Sensor)
    {
        sensorFrame->setStateText(QString("%1(ppm)").arg(co1Sensor->getCo1()));
        sensorFrame->getIconLabel()->setProperty("type", "co1_sensor");
    }
}

