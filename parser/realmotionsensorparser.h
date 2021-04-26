#ifndef REALMOTIONSENSORPARSER_H
#define REALMOTIONSENSORPARSER_H

#include "sensorparser.h"

class RealMotionSensorParser : public SensorParser
{
public:
    RealMotionSensorParser();

    void parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor);
    void updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor);
    void updateReferredSensorDataAndActionAfterUserDataNotifyUpdate(MeshModel *meshModel, Sensor *sensor);

    void parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame);
};

#endif // REALMOTIONSENSORPARSER_H
