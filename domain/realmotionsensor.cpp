#include "realmotionsensor.h"

RealMotionSensor::RealMotionSensor()
{

}

RealMotionSensor::~RealMotionSensor()
{

}

Sensor::SensorType RealMotionSensor::sensorType()
{
    return Sensor::RealMotionSensor;
}

QString RealMotionSensor::typeText()
{
    return REAL_MOTION_SENSOR_TYPE_TEXT;
}

QString RealMotionSensor::displayEnglishTypeText()
{
    return "Real Motion Sensor";
}

QString RealMotionSensor::shortTypeText()
{
    return REAL_MOTION_SENSOR_SHORT_TYPE_TEXT;
}

bool RealMotionSensor::isVirtual()
{
    return false;
}

QString RealMotionSensor::staticTypeText()
{
    return REAL_MOTION_SENSOR_TYPE_TEXT;
}

QString RealMotionSensor::dataText()
{
    return QObject::tr("STATUS: %1").arg(this->isChecked()?QObject::tr("TRIGGERED"):QObject::tr("NOT TRIGGERED"));
}
