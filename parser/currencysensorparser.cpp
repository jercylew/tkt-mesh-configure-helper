#include "currencysensorparser.h"
#include "domain/currencysensor.h"
#include "model/meshmodel.h"
#include "model/sensorsmodel.h"
#include "widget/sensorframe.h"

CurrencySensorParser::CurrencySensorParser()
{

}

void CurrencySensorParser::parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor)
{
    SensorParser::parseUserDataFrameToSensor(dataFrame, sensor);
    CurrencySensor *currencySensor = static_cast<CurrencySensor*>(sensor);
    if(currencySensor)
    {
        double dCurrency = 0.0;
        for (int i = 1;i <= 8;i++)
        {
            dCurrency += ((quint8)dataFrame.at(10 + i))/10.0;
        }
        dCurrency /= 8.0;

        currencySensor->setCurrency(dCurrency);
    }
}

void CurrencySensorParser::updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor)
{
    if(oldSensor == NULL)
    {
        return;
    }
    SensorParser::updateSensorFromUserDataNotify(meshModel, newSensor, oldSensor);
    CurrencySensor *oldCurrencySensor = static_cast<CurrencySensor *>(oldSensor);
    CurrencySensor *newCurrencySensor = static_cast<CurrencySensor *>(newSensor);

    oldCurrencySensor->setCurrency(newCurrencySensor->getCurrency());
}

void CurrencySensorParser::parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame)
{
    SensorParser::parseSensorToSensorFrame(sensor, sensorFrame);
    CurrencySensor *currencySensor = static_cast<CurrencySensor*>(sensor);
    if(currencySensor)
    {
        QString stateText = QString("%1A").arg(currencySensor->getCurrency());
        sensorFrame->setStateText(stateText);
        sensorFrame->getIconLabel()->setProperty("type", currencySensor->typeText());
    }
}

