#ifndef HANDWASHINGSENSOR_H
#define HANDWASHINGSENSOR_H

#include <domain/sensor.h>

#define HAND_WASHING_SENSOR_TYPE_TEXT           "hand_washing_sensor"
#define HAND_WASHING_SENSOR_SHORT_TYPE_TEXT     "HW"

class HandWashingSensor : public Sensor
{
public:
    HandWashingSensor();
    ~HandWashingSensor();

    SensorType sensorType();
    QString typeText();
    QString shortTypeText();
    QString displayEnglishTypeText();
    bool isVirtual();

    static QString staticTypeText();
    QString dataText();

    void setWashing(bool bIsWashing);
    bool getWashing() { return m_bIsWashing; }

private:
    bool    m_bIsWashing;
};

#endif // HANDWASHINGSENSOR_H
