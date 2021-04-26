#include "co2sensor.h"

CO2Sensor::CO2Sensor()
{
    setCo2(0);
}

CO2Sensor::~CO2Sensor()
{

}

void CO2Sensor::setCo2(quint16 co2)
{
    this->co2=co2;
}

quint16 CO2Sensor::getCo2()
{
    return this->co2;
}

Sensor::SensorType CO2Sensor::sensorType()
{
    return Sensor::CO2Sensor;
}

QString CO2Sensor::typeText()
{
    return CO2_SENSOR_TYPE_TEXT;
}

QString CO2Sensor::displayEnglishTypeText()
{
    return "CO2 Sensor";
}

QString CO2Sensor::shortTypeText()
{
    return CO2_SENSOR_SHORT_TYPE_TEXT;
}

bool CO2Sensor::isVirtual()
{
    return false;
}

QString CO2Sensor::staticTypeText()
{
    return CO2_SENSOR_TYPE_TEXT;
}

QString CO2Sensor::dataText()
{
    return QObject::tr("CO2: %1").arg(this->getCo2());
}
