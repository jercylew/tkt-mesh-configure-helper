#ifndef VIRTUALSWITCHSENSOR_H
#define VIRTUALSWITCHSENSOR_H

#include "domain/realswitchsensor.h"
#include <QMap>

#define VIRTUAL_SWITCH_SENSOR_TYPE_TEXT     "virtual_switch_sensor"
#define VIRTUAL_SWITCH_SENSOR_SHORT_TYPE_TEXT     "VS"

class VirtualSwitchSensor : public Sensor
{
public:
    VirtualSwitchSensor();
    ~VirtualSwitchSensor();

    bool isVirtual();
    SensorType sensorType();
    QString typeText();
    QString displayEnglishTypeText();
    QString shortTypeText();

    QString getChannelControlTypeText(int channel);
    void setChannelControlTypeText(int channel, const QString &controlTypeText);

    void setCurrentTrigValue(int trigValue);
    int currentTrigValue();
    QString currentTrigValueDisplayText();
    RealSwitchSensor::SwitchControlType currentTrigControlType();

    static QString staticTypeText();

    QList<quint8> getLogicGroupIDList();
    void setLogicGroupIDList(const QList<quint8> &logicGroupIDList);

    QMap<int, QString> getChannelControlTypeTextMap();
    void setChannelControlTypeTextMap(const QMap<int, QString> &channelControlTypeTextMap);

    QString getBindingSwitchSensorId();
    void setBindingSwitchSensorId(const QString &bindingSwitchSensorId);

    QString dataText();

private:
    QMap<int, QString> m_channelControlTypeTextMap;
    QString m_bindingSwitchSensorId;
    QList<quint8> m_logicGroupIDList;
    int m_currentTrigValue;
};

#endif // VIRTUALSWITCHSENSOR_H
