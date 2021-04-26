#include "temperaturehumiditysensor.h"

TemperatureHumiditySensor::TemperatureHumiditySensor()
    : Sensor()
{
    m_temperature=0;
    m_humidity=0;
}

float TemperatureHumiditySensor::temperature()
{
    return this->m_temperature;
}

void TemperatureHumiditySensor::setTemperature(float temperature)
{
    this->m_temperature=temperature;
}

float TemperatureHumiditySensor::humidity()
{
    return this->m_humidity;
}

void TemperatureHumiditySensor::setHumidity(float humidity)
{
    this->m_humidity=humidity;
}

Sensor::SensorType TemperatureHumiditySensor::sensorType()
{
    return Sensor::TemperatureHumiditySensor;
}

QString TemperatureHumiditySensor::typeText()
{
    return TEMPERATURE_HUMIDITY_SENSOR_TYPE_TEXT;
}

QString TemperatureHumiditySensor::displayEnglishTypeText()
{
    return "Temperature-Humidity Sensor";
}

QString TemperatureHumiditySensor::shortTypeText()
{
    return TEMPERATURE_HUMIDITY_SENSOR_SHORT_TYPE_TEXT;
}

bool TemperatureHumiditySensor::isVirtual()
{
    return false;
}

QString TemperatureHumiditySensor::staticTypeText()
{
    return TEMPERATURE_HUMIDITY_SENSOR_TYPE_TEXT;
}

QString TemperatureHumiditySensor::dataText()
{
    return QObject::tr("TEMP: %1, HUMI: %2")
            .arg(this->temperature())
            .arg(this->humidity());
}
