#ifndef STRESSSENSOR_H
#define STRESSSENSOR_H

#include "sensor.h"

#define STRESS_SENSOR_TYPE_TEXT "stress_sensor"
#define STRESS_SENSOR_SHORT_TYPE_TEXT "STR"

class StressSensor : public Sensor
{
public:
    StressSensor();

    int stress();
    void setStress(int stress);
    bool triggered();
    void setTriggered(bool triggered);

    int deltaStress();
    void setDeltaStress(int deltaStress);
    bool deltaStressTriggered();
    void setDeltaStressTriggered(bool deltaStressTriggered);

    Sensor::SensorType sensorType();
    QString typeText();
    QString displayEnglishTypeText();
    QString shortTypeText();
    bool isVirtual();

    static QString staticTypeText();
    QString dataText();

private:
    int m_stress;
    bool m_triggered;
    int m_deltaStress;
    bool m_deltaStressTriggered;
};

#endif // STRESSSENSOR_H
