#include "realswitchsensor.h"

QMap<RealSwitchSensor::SwitchControlType, QString> *RealSwitchSensor::m_switchControlTypeToTextMap=NULL;
QMap<QString, RealSwitchSensor::SwitchControlType> *RealSwitchSensor::m_switchControlTypeTextToTypeMap=NULL;

RealSwitchSensor::RealSwitchSensor()
{
    m_trigValue=0;
    m_bindGroupId=0;
}

RealSwitchSensor::~RealSwitchSensor()
{

}

Sensor::SensorType RealSwitchSensor::sensorType()
{
    return SensorType::RealSwitchSensor;
}

QString RealSwitchSensor::typeText()
{
    return REAL_SWITCH_SENSOR_TYPE_TEXT;
}

QString RealSwitchSensor::displayEnglishTypeText()
{
    return "Real Switch Sensor";
}

QString RealSwitchSensor::shortTypeText()
{
    return REAL_SWITCH_SENSOR_SHORT_TYPE_TEXT;
}

bool RealSwitchSensor::isVirtual()
{
    return false;
}

RealSwitchSensor::SwitchControlType RealSwitchSensor::getSwitchControlTypeByText(const QString &typeText)
{
    if(m_switchControlTypeTextToTypeMap==NULL)
    {
        m_switchControlTypeTextToTypeMap=new QMap<QString, RealSwitchSensor::SwitchControlType>;
        m_switchControlTypeTextToTypeMap->insert("switch_date_schedule", SwitchDateSchedule);
        m_switchControlTypeTextToTypeMap->insert("switch_onduty_schedule", SwitchOnDutySchedule);
        m_switchControlTypeTextToTypeMap->insert("switch_offduty_schedule", SwitchOffDutySchedule);
        m_switchControlTypeTextToTypeMap->insert("switch_schedule_off", SwitchScheduleOff);
        m_switchControlTypeTextToTypeMap->insert("switch_none", SwitchNone);
    }
    if(m_switchControlTypeTextToTypeMap->contains(typeText))
    {
        return m_switchControlTypeTextToTypeMap->value(typeText);
    }
    else
    {
        return SwitchNone;
    }
}

QList<QString> RealSwitchSensor::getWitchControlTypeTextList()
{
    QList<QString> typeTextList;
    typeTextList.push_back("switch_date_schedule");
    typeTextList.push_back("switch_onduty_schedule");
    typeTextList.push_back("switch_offduty_schedule");
    typeTextList.push_back("switch_schedule_off");
    typeTextList.push_back("switch_none");
    return typeTextList;
}

void RealSwitchSensor::setTrigValue(int trigValue)
{
    this->m_trigValue=trigValue;
}

int RealSwitchSensor::trigValue()
{
    return this->m_trigValue;
}

void RealSwitchSensor::setBindGroupId(quint8 bindGroupId)
{
    this->m_bindGroupId=bindGroupId;
}

quint8 RealSwitchSensor::bindGroupId()
{
    return this->m_bindGroupId;
}

QString RealSwitchSensor::staticTypeText()
{
    return REAL_SWITCH_SENSOR_TYPE_TEXT;
}

QString RealSwitchSensor::dataText()
{
    return QObject::tr("KEY: %1").arg(this->trigValue());
}

QString RealSwitchSensor::getSwitchControlTypeTextByType(RealSwitchSensor::SwitchControlType switchControlType)
{
    if(m_switchControlTypeToTextMap==NULL)
    {
        m_switchControlTypeToTextMap=new QMap<RealSwitchSensor::SwitchControlType, QString>;
        m_switchControlTypeToTextMap->insert(SwitchDateSchedule, "switch_date_schedule");
        m_switchControlTypeToTextMap->insert(SwitchOnDutySchedule, "switch_onduty_schedule");
        m_switchControlTypeToTextMap->insert(SwitchOffDutySchedule, "switch_offduty_schedule");
        m_switchControlTypeToTextMap->insert(SwitchScheduleOff, "switch_schedule_off");
        m_switchControlTypeToTextMap->insert(SwitchNone, "switch_none");
    }
    QString text = m_switchControlTypeToTextMap->value(switchControlType);
    if(text.isEmpty())
    {
        text = "switch_none";
    }
    return text;
}
