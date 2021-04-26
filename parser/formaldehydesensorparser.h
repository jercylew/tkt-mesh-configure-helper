#ifndef FORMALDEHYDESENSORPARSER_H
#define FORMALDEHYDESENSORPARSER_H

#include "sensorparser.h"

class FormaldehydeSensorParser : public SensorParser
{
public:
    FormaldehydeSensorParser();

    void parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor);
    void updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor);

    void parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame);
};

#endif // FORMALDEHYDESENSORPARSER_H
