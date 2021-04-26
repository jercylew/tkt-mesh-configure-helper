#include "luxsensor.h"

LuxSensor::LuxSensor()
{
    lux=0;
}

LuxSensor::~LuxSensor()
{

}

void LuxSensor::setLux(quint16 lux)
{
    this->lux=lux;
}

quint16 LuxSensor::getLux()
{
    return this->lux;
}

Sensor::SensorType LuxSensor::sensorType()
{
    return Sensor::LuxSensor;
}

QString LuxSensor::typeText()
{
    return LUX_SENSOR_TYPE_TEXT;
}

QString LuxSensor::displayEnglishTypeText()
{
    return "Lux Sensor";
}

QString LuxSensor::shortTypeText()
{
    return LUX_SENSOR_SHORT_TYPE_TEXT;
}

bool LuxSensor::isVirtual()
{
    return false;
}

QString LuxSensor::staticTypeText()
{
    return LUX_SENSOR_TYPE_TEXT;
}

void LuxSensor::pushLux(quint16 lux, int filterCoefficient)
{
    m_bufferedLuxList.push_back(lux);
    int tmpFilterCoefficient=filterCoefficient;
    if(tmpFilterCoefficient<3)
    {
        tmpFilterCoefficient=3;
    }
    else if(tmpFilterCoefficient>51)
    {
        tmpFilterCoefficient=51;
    }
    while(m_bufferedLuxList.size()>tmpFilterCoefficient)
    {
        m_bufferedLuxList.takeFirst();
    }
    if(m_bufferedLuxList.size()<=1)
    {
        setLux(lux);
    }
    else
    {
        QList<quint16> sortList=m_bufferedLuxList;
        qSort(sortList.begin(), sortList.end());
        setLux(sortList.at(sortList.size()/2));
    }
}

QString LuxSensor::dataText()
{
    return QObject::tr("LUX: %1").arg(getLux());
}
