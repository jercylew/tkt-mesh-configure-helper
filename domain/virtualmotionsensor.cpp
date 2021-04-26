#include "virtualmotionsensor.h"

VirtualMotionSensor::VirtualMotionSensor()
{

}

VirtualMotionSensor::~VirtualMotionSensor()
{

}

Sensor::SensorType VirtualMotionSensor::sensorType()
{
    return Sensor::VirtualMotionSensor;
}

QString VirtualMotionSensor::typeText()
{
    return VIRTUAL_MOTION_SENSOR_TYPE_TEXT;
}

QString VirtualMotionSensor::displayEnglishTypeText()
{
    return "Virtual Motion Sensor";
}

QString VirtualMotionSensor::shortTypeText()
{
    return VIRTUAL_MOTION_SENSOR_SHORT_TYPE_TEXT;
}

bool VirtualMotionSensor::isVirtual()
{
    return true;
}

QString VirtualMotionSensor::staticTypeText()
{
    return VIRTUAL_MOTION_SENSOR_TYPE_TEXT;
}

QList<QString> VirtualMotionSensor::bindingRealMotionSensorIdList()
{
    return m_bindingRealMotionSensorIdList;
}

void VirtualMotionSensor::setBindingRealMotionSensorIdList(const QList<QString> &bindingRealMotionSensorIdList)
{
    m_bindingRealMotionSensorIdList = bindingRealMotionSensorIdList;
}

QString VirtualMotionSensor::dataText()
{
    return QString("BINDING MOTION SENSORS: %1").arg(this->m_bindingRealMotionSensorIdList.size());
}
