#ifndef TEMPERATUREHUMIDITYSENSOR_H
#define TEMPERATUREHUMIDITYSENSOR_H

#include "sensor.h"

#define TEMPERATURE_HUMIDITY_SENSOR_TYPE_TEXT    "temp_hum_sensor"
#define TEMPERATURE_HUMIDITY_SENSOR_SHORT_TYPE_TEXT    "TH"

class TemperatureHumiditySensor : public Sensor
{
public:
    TemperatureHumiditySensor();

    float temperature();
    void setTemperature(float temperature);
    float humidity();
    void setHumidity(float humidity);

    Sensor::SensorType sensorType();
    QString typeText();
    QString displayEnglishTypeText();
    QString shortTypeText();
    bool isVirtual();

    Sensor *copyAll();

    static QString staticTypeText();
    QString dataText();

private:
    float m_temperature;
    float m_humidity;
};

#endif // TEMPERATUREHUMIDITYSENSOR_H
