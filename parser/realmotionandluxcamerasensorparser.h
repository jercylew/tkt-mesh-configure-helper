#ifndef REALMOTIONANDLUXCAMERASENSORPARSER_H
#define REALMOTIONANDLUXCAMERASENSORPARSER_H

#include "sensorparser.h"

class RealMotionAndLuxCameraSensorParser : public SensorParser
{
public:
    RealMotionAndLuxCameraSensorParser();

    void parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor);
    void updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor);
    void updateReferredSensorDataAndNotifyAfterOnlineStatusUpdate(MeshModel *meshModel, Sensor *sensor);
    void updateReferredSensorDataAndActionAfterUserDataNotifyUpdate(MeshModel *meshModel, Sensor *sensor);

    void parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame);
};

#endif // REALMOTIONANDLUXCAMERASENSORPARSER_H
