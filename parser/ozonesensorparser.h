#ifndef OZONESENSORPARSER_H
#define OZONESENSORPARSER_H

#include "sensorparser.h"

class OzoneSensorParser : public SensorParser
{
public:
    OzoneSensorParser();

    void parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor);
    void updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor);

    void parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame);
};

#endif // OZONESENSORPARSER_H
