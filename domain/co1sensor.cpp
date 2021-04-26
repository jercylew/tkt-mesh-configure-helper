#include "co1sensor.h"

CO1Sensor::CO1Sensor()
{
    setCo1(0);
}

CO1Sensor::~CO1Sensor()
{

}

void CO1Sensor::setCo1(quint16 co1)
{
    this->co1=co1;
}

quint16 CO1Sensor::getCo1()
{
    return this->co1;
}

Sensor::SensorType CO1Sensor::sensorType()
{
    return Sensor::CO1Sensor;
}

QString CO1Sensor::typeText()
{
    return CO1_SENSOR_TYPE_TEXT;
}

QString CO1Sensor::displayEnglishTypeText()
{
    return "CO1 Sensor";
}

QString CO1Sensor::shortTypeText()
{
    return CO1_SENSOR_SHORT_TYPE_TEXT;
}

bool CO1Sensor::isVirtual()
{
    return false;
}

QString CO1Sensor::staticTypeText()
{
    return CO1_SENSOR_TYPE_TEXT;
}

QString CO1Sensor::dataText()
{
    return QObject::tr("CO1: %1").arg(this->getCo1());
}
