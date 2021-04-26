#include "stresssensor.h"


StressSensor::StressSensor()
{
    m_stress=0;
    m_triggered=false;
    m_deltaStress=0;
    m_deltaStressTriggered=false;
}

int StressSensor::stress()
{
    return this->m_stress;
}

void StressSensor::setStress(int stress)
{
    this->m_stress=stress;
}

bool StressSensor::triggered()
{
    return this->m_triggered;
}

void StressSensor::setTriggered(bool triggered)
{
    this->m_triggered=triggered;
}

int StressSensor::deltaStress()
{
    return m_deltaStress;
}

void StressSensor::setDeltaStress(int deltaStress)
{
    m_deltaStress = deltaStress;
}

bool StressSensor::deltaStressTriggered()
{
    return m_deltaStressTriggered;
}

void StressSensor::setDeltaStressTriggered(bool deltaStressTriggered)
{
    m_deltaStressTriggered = deltaStressTriggered;
}

Sensor::SensorType StressSensor::sensorType()
{
    return Sensor::StressSensor;
}

QString StressSensor::typeText()
{
    return STRESS_SENSOR_TYPE_TEXT;
}

QString StressSensor::displayEnglishTypeText()
{
    return "Stress Sensor";
}

QString StressSensor::shortTypeText()
{
    return STRESS_SENSOR_SHORT_TYPE_TEXT;
}

bool StressSensor::isVirtual()
{
    return false;
}

QString StressSensor::staticTypeText()
{
    return STRESS_SENSOR_TYPE_TEXT;
}

QString StressSensor::dataText()
{
    return QObject::tr("STRESS: %1")
            .arg(this->stress());
}
