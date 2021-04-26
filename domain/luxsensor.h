#ifndef LUXSENSOR_H
#define LUXSENSOR_H

#include "domain/sensor.h"

#define LUX_SENSOR_TYPE_TEXT    "lux_sensor"
#define LUX_SENSOR_SHORT_TYPE_TEXT    "LUX"

class LuxSensor : public Sensor
{
public:
    LuxSensor();
    ~LuxSensor();

    void setLux(quint16 lux);
    quint16 getLux();
    Sensor::SensorType sensorType();
    QString typeText();
    QString displayEnglishTypeText();
    QString shortTypeText();
    bool isVirtual();

    static QString staticTypeText();
    void pushLux(quint16 lux, int filterCoefficient);

    QString dataText();

private:
    quint16 lux;
    QList<quint16> m_bufferedLuxList;
};

#endif // LUXSENSOR_H
