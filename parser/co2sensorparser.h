#ifndef CO2SENSORPARSER_H
#define CO2SENSORPARSER_H

#include "sensorparser.h"

class CO2SensorParser : public SensorParser
{
public:
    CO2SensorParser();

    void parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor);
    void updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor);

    void parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame);
};

#endif // CO2SENSORPARSER_H
