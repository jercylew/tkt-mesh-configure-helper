#ifndef VIRTUALMOTIONSENSOR_H
#define VIRTUALMOTIONSENSOR_H

#include "abstractmotionsensor.h"
#include <QList>

#define VIRTUAL_MOTION_SENSOR_TYPE_TEXT     "virtual_motion_sensor"
#define VIRTUAL_MOTION_SENSOR_SHORT_TYPE_TEXT   "VM"

class VirtualMotionSensor : public AbstractMotionSensor
{
public:
    VirtualMotionSensor();
    ~VirtualMotionSensor();

    SensorType sensorType();
    QString typeText();
    QString displayEnglishTypeText();
    QString shortTypeText();
    bool isVirtual();

    static QString staticTypeText();

    QList<QString> bindingRealMotionSensorIdList();
    void setBindingRealMotionSensorIdList(const QList<QString> &bindingRealMotionSensorIdList);

    QString dataText();

private:
    QList<QString> m_bindingRealMotionSensorIdList;
};

#endif // VIRTUALMOTIONSENSOR_H
