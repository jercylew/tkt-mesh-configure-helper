#ifndef FORMALDEHYDESENSOR_H
#define FORMALDEHYDESENSOR_H

#include "sensor.h"

#define FORMALDEHYDE_SENSOR_TYPE_TEXT   "formaldehyde_sensor"
#define FORMALDEHYDE_SENSOR_SHORT_TYPE_TEXT   "FOR"

class FormaldehydeSensor : public Sensor
{
public:
    FormaldehydeSensor();

    Sensor::SensorType sensorType();
    QString typeText();
    QString displayEnglishTypeText();
    QString shortTypeText();
    bool isVirtual();

    static QString staticTypeText();
    QString dataText();

    int formaldehyde();
    void setFormaldehyde(int formaldehyde);

private:
    int m_formaldehyde;
};

#endif // FORMALDEHYDESENSOR_H
