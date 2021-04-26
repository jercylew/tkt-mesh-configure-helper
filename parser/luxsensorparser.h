#ifndef LUXSENSORPARSER_H
#define LUXSENSORPARSER_H

#include "sensorparser.h"

class LuxSensorParser : public SensorParser
{
public:
    LuxSensorParser();

    void parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor);
    void updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor);
    void updateReferredSensorDataAndActionAfterUserDataNotifyUpdate(MeshModel *meshModel, Sensor *sensor);

    void parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame);
};

#endif // LUXSENSORPARSER_H
