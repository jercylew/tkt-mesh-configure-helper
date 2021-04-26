#ifndef REALMOTIONANDLUXCAMERASENSOR_H
#define REALMOTIONANDLUXCAMERASENSOR_H

#include "sensor.h"

#define REAL_MOTION_LUX_CAMERA_TYPE_TEXT    "real_motion_lux_camera"
#define REAL_MOTION_LUX_CAMERA_SHORT_TYPE_TEXT    "RML"

class RealMotionAndLuxCameraSensor : public Sensor
{
public:
    RealMotionAndLuxCameraSensor();

    Sensor::SensorType sensorType();
    QString typeText();
    QString displayEnglishTypeText();
    QString shortTypeText();
    bool isVirtual();

    void setMotionChannelTrigged(int channel, bool trigged);
    bool motionChannelTrigged(int channel);

    void setLuxChannelValue(int channel, quint8 value);
    quint8 luxChannelValue(int channel);

    void setMotionChannelTrigMap(const QMap<int, bool> &motionChannelTrigMap);
    QMap<int, bool> motionChannelTrigMap();

    void setMotionChannelLuxMap(const QMap<int, quint8> &motionChannelLuxMap);
    QMap<int, quint8> motionChannelLuxMap();

    static QString staticTypeText();

    void setApEnabled(bool apEnabled);
    bool apEnabled();

    QString dataText();

private:
    QMap<int, bool> m_motionChannelTrigMap;
    QMap<int, quint8> m_motionChannelLuxMap;
    bool m_apEnabled;
};

#endif // REALMOTIONANDLUXCAMERASENSOR_H
