#ifndef REALSWITCHSENSOR2CH_H
#define REALSWITCHSENSOR2CH_H

#include "domain/sensor.h"

#define REAL_SWITCH_SENSOR_2CH_TYPE_TEXT  "real_switch_sensor_2ch"
#define REAL_SWITCH_SENSOR_2CH_SHORT_TYPE_TEXT  "RS2"

class RealSwitchSensor2CH : public Sensor
{
public:
    typedef enum{
        SwitchDateSchedule,
        SwitchOnDutySchedule,
        SwitchOffDutySchedule,
        SwitchScheduleOff,
        SwitchNone
    }SwitchControlType;

    RealSwitchSensor2CH();
    ~RealSwitchSensor2CH();

    Sensor::SensorType sensorType();
    QString typeText();
    QString displayEnglishTypeText();
    QString shortTypeText();
    bool isVirtual();

    static QString getSwitchControlTypeTextByType(SwitchControlType switchControlType);
    static SwitchControlType getSwitchControlTypeByText(const QString &typeText);
    static QList<QString> getWitchControlTypeTextList();

    static QMap<SwitchControlType, QString> *m_switchControlTypeToTextMap;
    static QMap<QString, SwitchControlType> *m_switchControlTypeTextToTypeMap;

    void setTrigValue(int trigValue);
    int trigValue();

    void setBindGroupId(quint8 bindGroupId);
    quint8 bindGroupId();

    static QString staticTypeText();
    QString dataText();

private:
    int m_trigValue;
    quint8 m_bindGroupId;

};

#endif // REALSWITCHSENSOR2CH_H
