#ifndef REFRGTEMPERATUREHUMDITYSENSORPARSER_H
#define REFRGTEMPERATUREHUMDITYSENSORPARSER_H

#include "sensorparser.h"

class RefrgTemperatureHumditySensorParser : public SensorParser
{
public:
    RefrgTemperatureHumditySensorParser();

    void parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor);

    void updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor);
    void parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame);
};

#endif // REFRGTEMPERATUREHUMDITYSENSORPARSER_H
