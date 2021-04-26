#include "refrgtemperaturehumiditysensor.h"

RefrgTemperatureHumiditySensor::RefrgTemperatureHumiditySensor()
    : Sensor()
{
    m_temperature[0] = m_temperature[1] = 0;
    m_humidity[0] = m_humidity[1] = 0;
}

float RefrgTemperatureHumiditySensor::temperature(int ch)
{
    return (ch >= 0 && ch < 2) ? m_temperature[ch] : 0.0f;
}

void RefrgTemperatureHumiditySensor::setTemperature(float temperature, int ch)
{
    if (ch >= 0 && ch < 2)
    {
        m_temperature[ch] = temperature;
    }
}

float RefrgTemperatureHumiditySensor::humidity(int ch)
{
    return (ch >= 0 && ch < 2) ? m_humidity[ch] : 0.0f;
}

void RefrgTemperatureHumiditySensor::setHumidity(float humidity, int ch)
{
    if (ch >= 0 && ch < 2)
    {
        m_humidity[ch] = humidity;
    }
}

float RefrgTemperatureHumiditySensor::voltage()
{
    return m_voltage;
}

void RefrgTemperatureHumiditySensor::setVoltage(float voltage)
{
    m_voltage = voltage;
}

Sensor::SensorType RefrgTemperatureHumiditySensor::sensorType()
{
    return Sensor::RefrgTemperatureHumiditySensor;
}

QString RefrgTemperatureHumiditySensor::typeText()
{
    return REFRG_TEMPERATURE_HUMIDITY_SENSOR_TYPE_TEXT;
}

QString RefrgTemperatureHumiditySensor::shortTypeText()
{
    return REFRG_TEMPERATURE_HUMIDITY_SENSOR_SHORT_TYPE_TEXT;
}

bool RefrgTemperatureHumiditySensor::isVirtual()
{
    return false;
}

QString RefrgTemperatureHumiditySensor::staticTypeText()
{
    return REFRG_TEMPERATURE_HUMIDITY_SENSOR_TYPE_TEXT;
}

QString RefrgTemperatureHumiditySensor::displayEnglishTypeText()
{
    return "Refrigerator Temperature-Humidity Sensor";
}

QString RefrgTemperatureHumiditySensor::dataText()
{

    return QObject::tr("RefrgTemperatureHumiditySensor.dateTextFormat")
            .arg(temperature(0))
            .arg(temperature(1))
            .arg(humidity(0))
            .arg(humidity(1))
            .arg(voltage());
}
