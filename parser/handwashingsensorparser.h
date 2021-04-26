#ifndef HANDWASHINGSENSORPARSER_H
#define HANDWASHINGSENSORPARSER_H

#include <parser/sensorparser.h>

class HandWashingSensorParser : public SensorParser
{
public:
    HandWashingSensorParser();
    virtual ~HandWashingSensorParser();

    void parseUserDataFrameToSensor(const QByteArray &dataFrame,
                                    Sensor *sensor);
    void updateSensorFromUserDataNotify(MeshModel *meshModel,
                                        Sensor *newSensor, Sensor *oldSensor);
    void parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame);
};

#endif // HANDWASHINGSENSORPARSER_H
