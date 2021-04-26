#ifndef PM25SENSOR_H
#define PM25SENSOR_H

#include "sensor.h"

#define PM_SENSOR_TYPE_TEXT     "pm_sensor"
#define PM_SENSOR_SHORT_TYPE_TEXT     "P25"

class PMSensor : public Sensor
{
public:
    PMSensor();
    ~PMSensor();

    void setPm25(quint16 pm25);
    quint16 getPm25();
    Sensor::SensorType sensorType();
    QString typeText();
    QString displayEnglishTypeText();
    QString shortTypeText();
    bool isVirtual();

    static QString staticTypeText();

    QString dataText();

private:
    quint16 pm25;
};

#endif // PM25SENSOR_H
