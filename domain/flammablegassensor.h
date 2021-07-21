#ifndef FLAMMABLEGASSENSOR_H
#define FLAMMABLEGASSENSOR_H

#include "sensor.h"

#define FLAMMABLE_GAS_SENSOR_TYPE_TEXT   "flammable_gas_sensor"
#define FLAMMABLE_GAS_SENSOR_SHORT_TYPE_TEXT   "FLM"

class FlammableGasSensor : public Sensor
{
public:
    FlammableGasSensor();

    Sensor::SensorType sensorType();
    QString typeText();
    QString shortTypeText();
    QString displayEnglishTypeText();
    bool isVirtual();

    static QString staticTypeText();
    QString dataText();

    double concentration();
    void setConcentration(double concentration);

private:
    double m_dConcentration;
};

#endif // FLAMMABLEGASSENSOR_H
