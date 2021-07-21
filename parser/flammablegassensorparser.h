#ifndef FLAMMABLEGASSENSORPARSER_H
#define FLAMMABLEGASSENSORPARSER_H

#include "sensorparser.h"

class FlammableGasSensorParser : public SensorParser
{
public:
    FlammableGasSensorParser();
    void parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor);
    void updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor);

    void parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame);
};

#endif // FLAMMABLEGASSENSORPARSER_H
