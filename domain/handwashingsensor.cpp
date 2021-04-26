#include "handwashingsensor.h"

HandWashingSensor::HandWashingSensor()
{
    m_bIsWashing = false;
}

HandWashingSensor::~HandWashingSensor()
{

}

Sensor::SensorType HandWashingSensor::sensorType()
{
    return Sensor::HandWashingSensor;
}

QString HandWashingSensor::typeText()
{
    return HAND_WASHING_SENSOR_TYPE_TEXT;
}

QString HandWashingSensor::shortTypeText()
{
    return HAND_WASHING_SENSOR_SHORT_TYPE_TEXT;
}

bool HandWashingSensor::isVirtual()
{
    return false;
}

QString HandWashingSensor::staticTypeText()
{
    return HAND_WASHING_SENSOR_TYPE_TEXT;
}

QString HandWashingSensor::dataText()
{
    QString dataText = QObject::tr("STATUS: %1")
            .arg(m_bIsWashing ?
                     QObject::tr("HandWashingSensor.Washing") :
                     QObject::tr("HandWashingSensor.Stopped"));
    return dataText;
}

QString HandWashingSensor::displayEnglishTypeText()
{
    return "Hand Washing Sensor";
}

void HandWashingSensor::setWashing(bool bIsWashing)
{
    m_bIsWashing = bIsWashing;
}
