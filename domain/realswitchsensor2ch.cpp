#include "realswitchsensor2ch.h"


QMap<RealSwitchSensor2CH::SwitchControlType, QString> *RealSwitchSensor2CH::m_switchControlTypeToTextMap=NULL;
QMap<QString, RealSwitchSensor2CH::SwitchControlType> *RealSwitchSensor2CH::m_switchControlTypeTextToTypeMap=NULL;

RealSwitchSensor2CH::RealSwitchSensor2CH()
{
    m_trigValue=0;
    m_bindGroupId=0;
}

RealSwitchSensor2CH::~RealSwitchSensor2CH()
{

}

Sensor::SensorType RealSwitchSensor2CH::sensorType()
{
    return SensorType::RealSwitchSensor2CH;
}

QString RealSwitchSensor2CH::typeText()
{
    return REAL_SWITCH_SENSOR_2CH_TYPE_TEXT;
}

QString RealSwitchSensor2CH::displayEnglishTypeText()
{
    return "Real Switch Sensor with 2 Channels";
}

QString RealSwitchSensor2CH::shortTypeText()
{
    return REAL_SWITCH_SENSOR_2CH_SHORT_TYPE_TEXT;
}

bool RealSwitchSensor2CH::isVirtual()
{
    return false;
}

RealSwitchSensor2CH::SwitchControlType RealSwitchSensor2CH::getSwitchControlTypeByText(const QString &typeText)
{
    if(m_switchControlTypeTextToTypeMap==NULL)
    {
        m_switchControlTypeTextToTypeMap=new QMap<QString, RealSwitchSensor2CH::SwitchControlType>;
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

QList<QString> RealSwitchSensor2CH::getWitchControlTypeTextList()
{
    QList<QString> typeTextList;
    typeTextList.push_back("switch_date_schedule");
    typeTextList.push_back("switch_onduty_schedule");
    typeTextList.push_back("switch_offduty_schedule");
    typeTextList.push_back("switch_schedule_off");
    typeTextList.push_back("switch_none");
    return typeTextList;
}

void RealSwitchSensor2CH::setTrigValue(int trigValue)
{
    this->m_trigValue=trigValue;
}

int RealSwitchSensor2CH::trigValue()
{
    return this->m_trigValue;
}

void RealSwitchSensor2CH::setBindGroupId(quint8 bindGroupId)
{
    this->m_bindGroupId=bindGroupId;
}

quint8 RealSwitchSensor2CH::bindGroupId()
{
    return this->m_bindGroupId;
}

QString RealSwitchSensor2CH::staticTypeText()
{
    return REAL_SWITCH_SENSOR_2CH_TYPE_TEXT;
}

QString RealSwitchSensor2CH::dataText()
{
    return QObject::tr("KEY: %1").arg(this->trigValue());
}

QString RealSwitchSensor2CH::getSwitchControlTypeTextByType(RealSwitchSensor2CH::SwitchControlType switchControlType)
{
    if(m_switchControlTypeToTextMap==NULL)
    {
        m_switchControlTypeToTextMap=new QMap<RealSwitchSensor2CH::SwitchControlType, QString>;
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
