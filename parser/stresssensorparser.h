#ifndef STRESSSENSORPARSER_H
#define STRESSSENSORPARSER_H

#include "sensorparser.h"

class StressSensorParser : public SensorParser
{
public:
    StressSensorParser();

    void parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor);
    void updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor);

    void parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame);
};

#endif // STRESSSENSORPARSER_H
