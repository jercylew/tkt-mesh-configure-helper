#ifndef TEMPERATUREHUMDITYSENSORPARSER_H
#define TEMPERATUREHUMDITYSENSORPARSER_H

#include "sensorparser.h"

class TemperatureHumditySensorParser : public SensorParser
{
public:
    TemperatureHumditySensorParser();

    void parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor);
    void updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor);

    void parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame);
};

#endif // TEMPERATUREHUMDITYSENSORPARSER_H
