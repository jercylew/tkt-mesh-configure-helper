#ifndef UVLIGHTSENSORPARSER_H
#define UVLIGHTSENSORPARSER_H

#include "sensorparser.h"

class UVLightSensorParser : public SensorParser
{
public:
    UVLightSensorParser();
    virtual ~UVLightSensorParser();

    void parseUserDataFrameToSensor(const QByteArray &dataFrame,
                                    Sensor *sensor);
    void updateSensorFromUserDataNotify(MeshModel *meshModel,
                                        Sensor *newSensor, Sensor *oldSensor);
    void parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame);
};

#endif // UVLIGHTSENSORPARSER_H
