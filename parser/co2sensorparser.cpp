#include "co2sensorparser.h"
#include "domain/co2sensor.h"
#include "widget/sensorframe.h"

CO2SensorParser::CO2SensorParser()
{

}

void CO2SensorParser::parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor)
{
    SensorParser::parseUserDataFrameToSensor(dataFrame, sensor);
    CO2Sensor *co2Sensor=static_cast<CO2Sensor*>(sensor);
    if(co2Sensor)
    {
        co2Sensor->setCo2((quint16)(((quint8)dataFrame.at(12))*256+(quint8)dataFrame.at(13)));
    }
}

void CO2SensorParser::updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor)
{
    if(oldSensor==NULL)
    {
        return;
    }
    SensorParser::updateSensorFromUserDataNotify(meshModel, newSensor, oldSensor);
    CO2Sensor *oldCO2Sensor=static_cast<CO2Sensor*>(oldSensor);
    CO2Sensor *newCO2Sensor=static_cast<CO2Sensor*>(newSensor);
    oldCO2Sensor->setCo2(newCO2Sensor->getCo2());
}

void CO2SensorParser::parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame)
{
    SensorParser::parseSensorToSensorFrame(sensor, sensorFrame);
    CO2Sensor *co2Sensor=static_cast<CO2Sensor*>(sensor);
    if(co2Sensor)
    {
        sensorFrame->setStateText(QString("%1(ppm)").arg(co2Sensor->getCo2()));
        sensorFrame->getIconLabel()->setProperty("type", "co2_sensor");
    }
}
