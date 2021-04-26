#ifndef UVLIGHTSENSOR_H
#define UVLIGHTSENSOR_H

#include <domain/sensor.h>

#define UVLIGHT_SENSOR_TYPE_TEXT            "uvlight_sensor"
#define UVLIGHT_SENSOR_SHORT_TYPE_TEXT      "UVL"

class UVLightSensor : public Sensor
{
public:
    UVLightSensor();
    ~UVLightSensor();

    Sensor::SensorType sensorType();
    QString typeText();
    QString shortTypeText();
    bool isVirtual();
    static QString staticTypeText();
    QString dataText();
    QString displayEnglishTypeText();

    void setSterilizing(bool isSterilizing);
    bool getSterilizing() { return m_bIsSterilizing; }

private:
    bool m_bIsSterilizing;
};

#endif // UVLIGHTSENSOR_H
