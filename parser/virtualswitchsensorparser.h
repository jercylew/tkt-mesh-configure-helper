#ifndef VIRTUALSWITCHSENSORPARSER_H
#define VIRTUALSWITCHSENSORPARSER_H

#include "sensorparser.h"

class VirtualSwitchSensorParser : public SensorParser
{
public:
    VirtualSwitchSensorParser();

    void parseSensorDataToSensor(const QString &sensorData, Sensor *sensor);
    QString buildSensorDataFromSensor(Sensor *sensor);
    void updateSensorFromDB(MeshModel *meshModel, Sensor *dbSensor, Sensor *oldSensor);

    void parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame);
};

#endif // VIRTUALSWITCHSENSORPARSER_H
