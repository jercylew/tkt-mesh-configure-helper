#include "flammablegassensor.h"

FlammableGasSensor::FlammableGasSensor()
{
    m_dConcentration = 0;
}

Sensor::SensorType FlammableGasSensor::sensorType()
{
    return Sensor::FlammableGasSensor;
}

QString FlammableGasSensor::typeText()
{
    return FLAMMABLE_GAS_SENSOR_TYPE_TEXT;
}

QString FlammableGasSensor::shortTypeText()
{
    return FLAMMABLE_GAS_SENSOR_SHORT_TYPE_TEXT;
}

bool FlammableGasSensor::isVirtual()
{
    return false;
}

QString FlammableGasSensor::staticTypeText()
{
    return FLAMMABLE_GAS_SENSOR_TYPE_TEXT;
}


QString FlammableGasSensor::displayEnglishTypeText()
{
    return "Flammable Gas Sensor";
}

QString FlammableGasSensor::dataText()
{
    return QObject::tr("Concentration: %1%").arg(m_dConcentration);
}

double FlammableGasSensor::concentration()
{
    return m_dConcentration;
}

void FlammableGasSensor::setConcentration(double concentration)
{
   m_dConcentration = concentration;
}
