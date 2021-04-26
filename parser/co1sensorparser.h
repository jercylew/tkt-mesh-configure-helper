#ifndef CO1SENSORPARSER_H
#define CO1SENSORPARSER_H

#include "sensorparser.h"

class CO1SensorParser : public SensorParser
{
public:
    CO1SensorParser();

    void parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor);
    void updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor);

    void parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame);
};

#endif // CO1SENSORPARSER_H
