#ifndef PMSENSORPARSER_H
#define PMSENSORPARSER_H

#include "sensorparser.h"

class PMSensorParser : public SensorParser
{
public:
    PMSensorParser();

    void parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor);
    void updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor);

    void parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame);
};

#endif // PMSENSORPARSER_H
