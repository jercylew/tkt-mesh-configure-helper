#include "uvlightsensor.h"

UVLightSensor::UVLightSensor()
{
    m_bIsSterilizing = false;
}

UVLightSensor::~UVLightSensor()
{

}

Sensor::SensorType UVLightSensor::sensorType()
{
    return Sensor::UVLightSensor;
}

QString UVLightSensor::typeText()
{
    return UVLIGHT_SENSOR_TYPE_TEXT;
}

QString UVLightSensor::shortTypeText()
{
    return UVLIGHT_SENSOR_SHORT_TYPE_TEXT;
}

bool UVLightSensor::isVirtual()
{
    return false;
}

QString UVLightSensor::staticTypeText()
{
    return UVLIGHT_SENSOR_TYPE_TEXT;
}

QString UVLightSensor::dataText()
{
    QString dataText = QObject::tr("STATUS: %1")
            .arg(m_bIsSterilizing ?
                     QObject::tr("UVLightSensor.Sterilizing") :
                     QObject::tr("UVLightSensor.Stopped"));
    return dataText;
}

QString UVLightSensor::displayEnglishTypeText()
{
    return "Ultraviolet Light Sensor";
}

void UVLightSensor::setSterilizing(bool isSterilizing)
{
    m_bIsSterilizing = isSterilizing;
}
