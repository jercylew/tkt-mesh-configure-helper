#include "realmotionandluxcamerasensor.h"

RealMotionAndLuxCameraSensor::RealMotionAndLuxCameraSensor()
{
    m_apEnabled=false;
}

Sensor::SensorType RealMotionAndLuxCameraSensor::sensorType()
{
    return Sensor::RealMotionAndLuxCameraSensor;
}

QString RealMotionAndLuxCameraSensor::typeText()
{
    return REAL_MOTION_LUX_CAMERA_TYPE_TEXT;
}

QString RealMotionAndLuxCameraSensor::displayEnglishTypeText()
{
    return "Real Motion-Lux Camera";
}

QString RealMotionAndLuxCameraSensor::shortTypeText()
{
    return REAL_MOTION_LUX_CAMERA_SHORT_TYPE_TEXT;
}

bool RealMotionAndLuxCameraSensor::isVirtual()
{
    return false;
}

void RealMotionAndLuxCameraSensor::setMotionChannelTrigged(int channel, bool trigged)
{
    m_motionChannelTrigMap.insert(channel, trigged);
}

bool RealMotionAndLuxCameraSensor::motionChannelTrigged(int channel)
{
    if(m_motionChannelLuxMap.contains(channel))
    {
        return m_motionChannelTrigMap.value(channel);
    }
    else
    {
        return false;
    }
}

void RealMotionAndLuxCameraSensor::setLuxChannelValue(int channel, quint8 value)
{
    m_motionChannelLuxMap.insert(channel, value);
}

quint8 RealMotionAndLuxCameraSensor::luxChannelValue(int channel)
{
    if(m_motionChannelLuxMap.contains(channel))
    {
        return m_motionChannelLuxMap.value(channel);
    }
    else
    {
        return 0;
    }
}

void RealMotionAndLuxCameraSensor::setMotionChannelTrigMap(const QMap<int, bool> &motionChannelTrigMap)
{
    this->m_motionChannelTrigMap=motionChannelTrigMap;
}

QMap<int, bool> RealMotionAndLuxCameraSensor::motionChannelTrigMap()
{
    return this->m_motionChannelTrigMap;
}

void RealMotionAndLuxCameraSensor::setMotionChannelLuxMap(const QMap<int, quint8> &motionChannelLuxMap)
{
    this->m_motionChannelLuxMap=motionChannelLuxMap;
}

QMap<int, quint8> RealMotionAndLuxCameraSensor::motionChannelLuxMap()
{
    return this->m_motionChannelLuxMap;
}

QString RealMotionAndLuxCameraSensor::staticTypeText()
{
    return REAL_MOTION_LUX_CAMERA_TYPE_TEXT;
}

void RealMotionAndLuxCameraSensor::setApEnabled(bool apEnabled)
{
    this->m_apEnabled=apEnabled;
}

bool RealMotionAndLuxCameraSensor::apEnabled()
{
    return this->m_apEnabled;
}

QString RealMotionAndLuxCameraSensor::dataText()
{
    return QString("%1/%2/%3/%4, %5/%6/%7/%8")
            .arg(this->motionChannelTrigged(1))
            .arg(this->motionChannelTrigged(2))
            .arg(this->motionChannelTrigged(3))
            .arg(this->motionChannelTrigged(4))
            .arg(this->luxChannelValue(1)&0xFF)
            .arg(this->luxChannelValue(2)&0xFF)
            .arg(this->luxChannelValue(3)&0xFF)
            .arg(this->luxChannelValue(4)&0xFF);
}


