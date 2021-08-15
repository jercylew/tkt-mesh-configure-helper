#ifndef CURRENCYSENSORPARSER_H
#define CURRENCYSENSORPARSER_H

#include "sensorparser.h"

class CurrencySensorParser : public SensorParser
{
public:
    CurrencySensorParser();

    void parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor);

    void updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor);

    void parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame);
};


#endif // CURRENCYSENSORPARSER_H
