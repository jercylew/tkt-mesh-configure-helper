#ifndef VIRTUALMOTIONSENSORPARSER_H
#define VIRTUALMOTIONSENSORPARSER_H

#include "sensorparser.h"

class VirtualMotionSensorParser : public SensorParser
{
public:
    VirtualMotionSensorParser();

    void parseSensorDataToSensor(const QString &sensorData, Sensor *sensor);
    QString buildSensorDataFromSensor(Sensor *sensor);
    void updateSensorFromDB(MeshModel *meshModel, Sensor *dbSensor, Sensor *oldSensor);
    void updateReferredSensorDataAndActionAfterDBUpdate(MeshModel *meshModel, Sensor *sensor);

    void parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame);
};

#endif // VIRTUALMOTIONSENSORPARSER_H
