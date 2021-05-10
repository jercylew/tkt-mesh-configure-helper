#include "ozonesensor.h"

OzoneSensor::OzoneSensor()
{
    m_nOzone = 0;
}

Sensor::SensorType OzoneSensor::sensorType()
{
    return Sensor::OzoneSensor;
}

QString OzoneSensor::typeText()
{
    return OZONE_SENSOR_TYPE_TEXT;
}

QString OzoneSensor::displayEnglishTypeText()
{
    return "Ozone Sensor";
}

QString OzoneSensor::shortTypeText()
{
    return OZONE_SENSOR_SHORT_TYPE_TEXT;
}

bool OzoneSensor::isVirtual()
{
    return false;
}

QString OzoneSensor::staticTypeText()
{
    return OZONE_SENSOR_TYPE_TEXT;
}

QString OzoneSensor::dataText()
{
    return QObject::tr("Ozone: %1").arg(ozone());
}

int OzoneSensor::ozone()
{
    return m_nOzone;
}

void OzoneSensor::setOzone(int ozone)
{
    m_nOzone = ozone;
}
