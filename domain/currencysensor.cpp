#include "currencysensor.h"

CurrencySensor::CurrencySensor()
    : Sensor()
{
    m_dCurrency = 0.0;
}

Sensor::SensorType CurrencySensor::sensorType()
{
    return Sensor::CurrencySensor;
}

QString CurrencySensor::typeText()
{
    return CURRENCY_SENSOR_TYPE_TEXT;
}

QString CurrencySensor::shortTypeText()
{
    return CURRENCY_SENSOR_SHORT_TYPE_TEXT;
}

bool CurrencySensor::isVirtual()
{
    return false;
}

QString CurrencySensor::staticTypeText()
{
    return CURRENCY_SENSOR_TYPE_TEXT;
}

QString CurrencySensor::dataText()
{
    return QObject::tr("CURRENCY: %1")
            .arg(m_dCurrency);
}

double CurrencySensor::dataValue(QString strValueName)
{
    strValueName = strValueName;
    return m_dCurrency;
}

void CurrencySensor::setCurrency(double dCurrency)
{
    m_dCurrency = dCurrency;
}

double CurrencySensor::getCurrency()
{
    return m_dCurrency;
}

QString CurrencySensor::displayEnglishTypeText()
{
    return "Currency Sensor";
}

