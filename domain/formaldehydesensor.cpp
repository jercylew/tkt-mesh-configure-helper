#include "formaldehydesensor.h"

FormaldehydeSensor::FormaldehydeSensor()
{
    this->m_formaldehyde=0;
}

Sensor::SensorType FormaldehydeSensor::sensorType()
{
    return Sensor::FormaldehydeSensor;
}

QString FormaldehydeSensor::typeText()
{
    return FORMALDEHYDE_SENSOR_TYPE_TEXT;
}

QString FormaldehydeSensor::displayEnglishTypeText()
{
    return "Formaldehyde Sensor";
}

QString FormaldehydeSensor::shortTypeText()
{
    return FORMALDEHYDE_SENSOR_SHORT_TYPE_TEXT;
}

bool FormaldehydeSensor::isVirtual()
{
    return false;
}

QString FormaldehydeSensor::staticTypeText()
{
    return FORMALDEHYDE_SENSOR_TYPE_TEXT;
}

QString FormaldehydeSensor::dataText()
{
    return QObject::tr("甲醛: %1").arg(this->formaldehyde());
}

int FormaldehydeSensor::formaldehyde()
{
    return m_formaldehyde;
}

void FormaldehydeSensor::setFormaldehyde(int formaldehyde)
{
    m_formaldehyde = formaldehyde;
}
