#ifndef REALSWITCHSENSORPARSER2CH_H
#define REALSWITCHSENSORPARSER2CH_H

#include "sensorparser.h"

class RealSwitchSensorParser2CH : public SensorParser
{
public:
    RealSwitchSensorParser2CH();

    void parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor);
    void updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor);
    void updateReferredSensorDataAndActionAfterUserDataNotifyUpdate(MeshModel *meshModel, Sensor *sensor);

    void parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame);
};

#endif // REALSWITCHSENSORPARSER2CH_H
