#ifndef OZONESENSOR_H
#define OZONESENSOR_H

#include "sensor.h"

#define OZONE_SENSOR_TYPE_TEXT   "ozone_sensor"
#define OZONE_SENSOR_SHORT_TYPE_TEXT   "OZO"

class OzoneSensor : public Sensor
{
public:
    OzoneSensor();

    Sensor::SensorType sensorType();
    QString typeText();
    QString displayEnglishTypeText();
    QString shortTypeText();
    bool isVirtual();

    static QString staticTypeText();
    QString dataText();

    int ozone();
    void setOzone(int ozone);

private:
    int m_nOzone;
};

#endif // OZONESENSOR_H
