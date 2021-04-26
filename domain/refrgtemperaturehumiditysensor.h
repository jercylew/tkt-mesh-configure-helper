#ifndef REFRGTEMPERATUREHUMIDITYSENSOR_H
#define REFRGTEMPERATUREHUMIDITYSENSOR_H

#include "sensor.h"

#define REFRG_TEMPERATURE_HUMIDITY_SENSOR_TYPE_TEXT    "refrg_temp_hum_sensor"
#define REFRG_TEMPERATURE_HUMIDITY_SENSOR_SHORT_TYPE_TEXT    "RTH"

class RefrgTemperatureHumiditySensor : public Sensor
{
public:
    RefrgTemperatureHumiditySensor();

    float temperature(int ch);
    void setTemperature(float temperature, int ch);
    float humidity(int ch);
    void setHumidity(float humidity, int ch);
    float voltage();
    void setVoltage(float voltage);

    Sensor::SensorType sensorType();
    QString typeText();
    QString shortTypeText();
    QString displayEnglishTypeText();
    bool isVirtual();

    Sensor *copyAll();

    static QString staticTypeText();
    QString dataText();

private:
    float m_temperature[2];     //2-channel temperature
    float m_humidity[2];        //2-channel humidity
    float m_voltage;            //Voltage (balance, <3.2v means need to charge)
};

#endif // REFRGTEMPERATUREHUMIDITYSENSOR_H
