#ifndef REALSWITCHSENSORPARSER_H
#define REALSWITCHSENSORPARSER_H

#include "sensorparser.h"

class RealSwitchSensorParser : public SensorParser
{
public:
    RealSwitchSensorParser();

    void parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor);
    void updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor);
    void updateReferredSensorDataAndActionAfterUserDataNotifyUpdate(MeshModel *meshModel, Sensor *sensor);

    void parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame);
};

#endif // REALSWITCHSENSORPARSER_H
