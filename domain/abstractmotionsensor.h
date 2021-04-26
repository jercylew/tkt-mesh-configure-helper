#ifndef ABSTRACTMOTIONSENSOR_H
#define ABSTRACTMOTIONSENSOR_H

#include "domain/sensor.h"

class AbstractMotionSensor : public Sensor
{
public:
    AbstractMotionSensor();

    void setChecked(bool checked);
    bool isChecked();

private:
    bool m_checked;

};

#endif // ABSTRACTMOTIONSENSOR_H
