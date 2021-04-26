#include "temperaturehumditysensorparser.h"
#include "domain/temperaturehumiditysensor.h"
#include "model/meshmodel.h"
#include "model/sensorsmodel.h"
#include "widget/sensorframe.h"

TemperatureHumditySensorParser::TemperatureHumditySensorParser()
{

}

void TemperatureHumditySensorParser::parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor)
{
    SensorParser::parseUserDataFrameToSensor(dataFrame, sensor);
    TemperatureHumiditySensor *tempHumSensor=static_cast<TemperatureHumiditySensor*>(sensor);
    if(tempHumSensor)
    {
        tempHumSensor->setTemperature((quint16)(((quint8)dataFrame.at(12))*256+(quint8)dataFrame.at(13))/10.0);
        tempHumSensor->setHumidity((quint16)(((quint8)dataFrame.at(14))*256+(quint8)dataFrame.at(15))/10.0);
    }
}

void TemperatureHumditySensorParser::updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor)
{
    if(oldSensor==NULL)
    {
        return;
    }
    SensorParser::updateSensorFromUserDataNotify(meshModel, newSensor, oldSensor);
    TemperatureHumiditySensor *oldTempHumSensor=static_cast<TemperatureHumiditySensor *>(oldSensor);
    TemperatureHumiditySensor *newTempHumSensor=static_cast<TemperatureHumiditySensor *>(newSensor);
    oldTempHumSensor->setTemperature(newTempHumSensor->temperature());
    oldTempHumSensor->setHumidity(newTempHumSensor->humidity());
}

void TemperatureHumditySensorParser::parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame)
{
    SensorParser::parseSensorToSensorFrame(sensor, sensorFrame);
    TemperatureHumiditySensor *tempHumSensor=static_cast<TemperatureHumiditySensor*>(sensor);
    if(tempHumSensor)
    {
        QString stateText=QString("%1℃/%2RH").arg(tempHumSensor->temperature()).arg(tempHumSensor->humidity());
        sensorFrame->setStateText(stateText);
        sensorFrame->getIconLabel()->setProperty("type", "temp_hum_sensor");
    }
}
