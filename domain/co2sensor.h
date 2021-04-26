#ifndef CO2SENSOR_H
#define CO2SENSOR_H

#include "sensor.h"

#define CO2_SENSOR_TYPE_TEXT    "co2_sensor"
#define CO2_SENSOR_SHORT_TYPE_TEXT    "CO2"

class CO2Sensor : public Sensor
{
public:
    CO2Sensor();
    ~CO2Sensor();

    void setCo2(quint16 co2);
    quint16 getCo2();
    Sensor::SensorType sensorType();
    QString typeText();
    QString displayEnglishTypeText();
    QString shortTypeText();
    bool isVirtual();

    static QString staticTypeText();
    QString dataText();

private:
    quint16 co2;
};

#endif // CO2SENSOR_H
