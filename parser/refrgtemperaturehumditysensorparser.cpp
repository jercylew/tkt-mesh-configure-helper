#include "refrgtemperaturehumditysensorparser.h"
#include "domain/refrgtemperaturehumiditysensor.h"
#include "model/meshmodel.h"
#include "model/sensorsmodel.h"
#include "widget/sensorframe.h"

RefrgTemperatureHumditySensorParser::RefrgTemperatureHumditySensorParser()
{

}

void RefrgTemperatureHumditySensorParser::parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor)
{
    SensorParser::parseUserDataFrameToSensor(dataFrame, sensor);
    RefrgTemperatureHumiditySensor *tempHumSensor=static_cast<RefrgTemperatureHumiditySensor*>(sensor);
    if(tempHumSensor)
    {
        qint16 nRawValue;
        nRawValue = ((quint8)dataFrame.at(12))*256+(quint8)dataFrame.at(13);
        //Temperature1 12-13
        tempHumSensor->setTemperature(nRawValue/10.0, 0);

        //Humidity1 14
        tempHumSensor->setHumidity(((quint8)dataFrame.at(14)), 0);

        //Temperature2 15-16
        nRawValue = ((quint8)dataFrame.at(15))*256+(quint8)dataFrame.at(16);
        tempHumSensor->setTemperature(nRawValue/10.0, 1);

        //Humidity2 17
        tempHumSensor->setHumidity(((quint8)dataFrame.at(17)), 1);

        //Voltage 18
        nRawValue = (quint8)dataFrame.at(18);
        tempHumSensor->setVoltage(nRawValue/10.0);
    }
}

void RefrgTemperatureHumditySensorParser::updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor)
{
    if(oldSensor==NULL)
    {
        return;
    }
    SensorParser::updateSensorFromUserDataNotify(meshModel, newSensor, oldSensor);
    RefrgTemperatureHumiditySensor *oldTempHumSensor = static_cast<RefrgTemperatureHumiditySensor *>(oldSensor);
    RefrgTemperatureHumiditySensor *newTempHumSensor = static_cast<RefrgTemperatureHumiditySensor *>(newSensor);

    for (int i = 0;i < 2;i++)
    {
        float oldTemp = oldTempHumSensor->temperature(i);
        float oldHum = oldTempHumSensor->humidity(i);

        oldTempHumSensor->setTemperature(newTempHumSensor->temperature(i), i);
        oldTempHumSensor->setHumidity(newTempHumSensor->humidity(i), i);
        if((oldTemp!=oldTempHumSensor->temperature(i)) || (oldHum!=oldTempHumSensor->humidity(i)))
        {
//            oldTempHumSensor->setDataChanged(true);
        }
    }

    float oldVol = oldTempHumSensor->voltage();
    oldTempHumSensor->setVoltage(newTempHumSensor->voltage());
    if((oldVol != oldTempHumSensor->voltage()))
    {
//        oldTempHumSensor->setDataChanged(true);
    }
}

void RefrgTemperatureHumditySensorParser::parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame)
{
    SensorParser::parseSensorToSensorFrame(sensor, sensorFrame);
    RefrgTemperatureHumiditySensor *tempHumSensor=static_cast<RefrgTemperatureHumiditySensor*>(sensor);
    if(tempHumSensor)
    {
        QString stateText=QString("%1℃,%2℃/%3RH,%4RH")
                .arg(tempHumSensor->temperature(0))
                .arg(tempHumSensor->temperature(1))
                .arg(tempHumSensor->humidity(0))
                .arg(tempHumSensor->humidity(1));
        sensorFrame->setStateText(stateText);
        sensorFrame->getIconLabel()->setProperty("type", "refrg_temp_hum_sensor");
    }
}
