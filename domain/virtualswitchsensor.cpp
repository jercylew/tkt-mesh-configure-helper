#include "virtualswitchsensor.h"
#include "realswitchsensor.h"

VirtualSwitchSensor::VirtualSwitchSensor()
{
    m_bindingSwitchSensorId="";
    m_currentTrigValue=0;
}

VirtualSwitchSensor::~VirtualSwitchSensor()
{

}

bool VirtualSwitchSensor::isVirtual()
{
    return true;
}

Sensor::SensorType VirtualSwitchSensor::sensorType()
{
    return SensorType::VirtualSwitchSensor;
}

QString VirtualSwitchSensor::typeText()
{
    return VIRTUAL_SWITCH_SENSOR_TYPE_TEXT;
}

QString VirtualSwitchSensor::displayEnglishTypeText()
{
    return "Virtual Switch Sensor";
}

QString VirtualSwitchSensor::shortTypeText()
{
    return VIRTUAL_SWITCH_SENSOR_SHORT_TYPE_TEXT;
}

QString VirtualSwitchSensor::getChannelControlTypeText(int channel)
{
    QString text=m_channelControlTypeTextMap.value(channel);
    if(text.isEmpty())
    {
        text="switch_none";
    }
    return text;
}

void VirtualSwitchSensor::setChannelControlTypeText(int channel, const QString &controlTypeText)
{
    m_channelControlTypeTextMap.insert(channel, controlTypeText);
}

void VirtualSwitchSensor::setCurrentTrigValue(int trigValue)
{
    this->m_currentTrigValue=trigValue;
}

int VirtualSwitchSensor::currentTrigValue()
{
    return this->m_currentTrigValue;
}

QString VirtualSwitchSensor::currentTrigValueDisplayText()
{
    return QObject::tr(QString("Global.%1").arg(getChannelControlTypeText(m_currentTrigValue)).toLatin1().data());
}

RealSwitchSensor::SwitchControlType VirtualSwitchSensor::currentTrigControlType()
{
    return RealSwitchSensor::getSwitchControlTypeByText(getChannelControlTypeText(m_currentTrigValue));
}

QString VirtualSwitchSensor::staticTypeText()
{
    return VIRTUAL_SWITCH_SENSOR_TYPE_TEXT;
}

void VirtualSwitchSensor::setLogicGroupIDList(const QList<quint8> &logicGroupIDList)
{
    this->m_logicGroupIDList=logicGroupIDList;
}

QMap<int, QString> VirtualSwitchSensor::getChannelControlTypeTextMap()
{
    return m_channelControlTypeTextMap;
}

void VirtualSwitchSensor::setChannelControlTypeTextMap(const QMap<int, QString> &channelControlTypeTextMap)
{
    m_channelControlTypeTextMap = channelControlTypeTextMap;
}

QString VirtualSwitchSensor::getBindingSwitchSensorId()
{
    return m_bindingSwitchSensorId;
}

void VirtualSwitchSensor::setBindingSwitchSensorId(const QString &bindingSwitchSensorId)
{
    m_bindingSwitchSensorId = bindingSwitchSensorId;
}

QString VirtualSwitchSensor::dataText()
{
    return QString("BINDING SENSOR: %1").arg(this->m_bindingSwitchSensorId);
}

QList<quint8> VirtualSwitchSensor::getLogicGroupIDList()
{
    return this->m_logicGroupIDList;
}
