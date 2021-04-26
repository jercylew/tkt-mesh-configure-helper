#ifndef REALMOTIONSENSOR_H
#define REALMOTIONSENSOR_H

#include "abstractmotionsensor.h"

#define REAL_MOTION_SENSOR_TYPE_TEXT    "real_motion_sensor"
#define REAL_MOTION_SENSOR_SHORT_TYPE_TEXT    "RM"

class RealMotionSensor : public AbstractMotionSensor
{
public:
    RealMotionSensor();
    ~RealMotionSensor();

    SensorType sensorType();
    QString typeText();
    QString displayEnglishTypeText();
    QString shortTypeText();
    bool isVirtual();

    static QString staticTypeText();
    QString dataText();
};

#endif // REALMOTIONSENSOR_H
