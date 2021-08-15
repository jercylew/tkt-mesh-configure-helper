#ifndef CURRENCYSENSOR_H
#define CURRENCYSENSOR_H

#include "sensor.h"

#define CURRENCY_SENSOR_TYPE_TEXT    "currency_sensor"
#define CURRENCY_SENSOR_SHORT_TYPE_TEXT    "CUR"

class CurrencySensor : public Sensor
{
public:
    CurrencySensor();

    void setCurrency(double dCurrency);
    double getCurrency();

    Sensor::SensorType sensorType();
    QString typeText();
    QString shortTypeText();
    bool isVirtual();

    static QString staticTypeText();
    QString dataText();
    double dataValue(QString strValueName = "");

private:
    double m_dCurrency;
};

#endif // CURRENCYSENSOR_H
