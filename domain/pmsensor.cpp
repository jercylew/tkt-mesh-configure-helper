#include "pmsensor.h"

PMSensor::PMSensor()
{
    setPm25(0);
}

PMSensor::~PMSensor()
{

}

void PMSensor::setPm25(quint16 pm25)
{
    this->pm25=pm25;
}

quint16 PMSensor::getPm25()
{
    return this->pm25;
}

Sensor::SensorType PMSensor::sensorType()
{
    return Sensor::PMSensor;
}

QString PMSensor::typeText()
{
    return PM_SENSOR_TYPE_TEXT;
}

QString PMSensor::displayEnglishTypeText()
{
    return "PM Sensor";
}

QString PMSensor::shortTypeText()
{
    return PM_SENSOR_SHORT_TYPE_TEXT;
}

bool PMSensor::isVirtual()
{
    return false;
}

QString PMSensor::staticTypeText()
{
    return PM_SENSOR_TYPE_TEXT;
}

QString PMSensor::dataText()
{
    return QString("PM2.5: %1").arg(this->getPm25());
}
