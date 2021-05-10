#ifndef SENSOR_H
#define SENSOR_H

#include "domain/bluetoothnode.h"

class Sensor : public BluetoothNode
{
public:
    enum SensorType
    {
        LuxSensor,
        RealMotionSensor,
        VirtualMotionSensor,
        PMSensor,
        CO1Sensor,
        CO2Sensor,
        RealSwitchSensor,
        RealSwitchSensor2CH,
        VirtualSwitchSensor,
        VirtualSwitchSensor2CH,
        RealMotionAndLuxCameraSensor,
        TemperatureHumiditySensor,
        StressSensor,
        FormaldehydeSensor,
        RefrgTemperatureHumiditySensor,
        UVLightSensor,
        HandWashingSensor,
        OzoneSensor
    };
    Sensor();
    virtual ~Sensor();

    virtual bool isVirtual()=0;
    virtual SensorType sensorType()=0;
    void setSensorId(const QString &sensorId);
    QString sensorId();

    QString nodeId();

    virtual QString nodeKindText();
    static QString staticNodeKindText();

    void loadSensorBasicDataFromDB(Sensor *sensor);
    void loadSensorBasicDataFromOnlineStatus(Sensor *sensor);
    void loadSensorBasicDataFromUserData(Sensor *sensor);

private:
    QString m_sensorId;
};

#endif // SENSOR_H
