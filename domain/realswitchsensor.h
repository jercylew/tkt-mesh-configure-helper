#ifndef REALSWITCHSENSOR_H
#define REALSWITCHSENSOR_H

#include "domain/sensor.h"

#define REAL_SWITCH_SENSOR_TYPE_TEXT  "real_switch_sensor"
#define REAL_SWITCH_SENSOR_SHORT_TYPE_TEXT  "RS"

class RealSwitchSensor : public Sensor
{
public:
    typedef enum{
        SwitchDateSchedule,
        SwitchOnDutySchedule,
        SwitchOffDutySchedule,
        SwitchScheduleOff,
        SwitchNone
    }SwitchControlType;

    RealSwitchSensor();
    ~RealSwitchSensor();

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

#endif // REALSWITCHSENSOR_H
