#ifndef CO1SENSOR_H
#define CO1SENSOR_H

#include "sensor.h"

#define CO1_SENSOR_TYPE_TEXT    "co1_sensor"
#define CO1_SENSOR_SHORT_TYPE_TEXT    "CO1"

class CO1Sensor : public Sensor
{
public:
    CO1Sensor();
    ~CO1Sensor();

    void setCo1(quint16 co1);
    quint16 getCo1();
    Sensor::SensorType sensorType();
    QString typeText();
    QString displayEnglishTypeText();
    QString shortTypeText();
    bool isVirtual();

    static QString staticTypeText();

    QString dataText();

private:
    quint16 co1;
};

#endif // CO1SENSOR_H
