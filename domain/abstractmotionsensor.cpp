#include "abstractmotionsensor.h"

AbstractMotionSensor::AbstractMotionSensor()
{
    setChecked(false);
}

void AbstractMotionSensor::setChecked(bool checked)
{
    this->m_checked=checked;
}

bool AbstractMotionSensor::isChecked()
{
    return this->m_checked;
}
