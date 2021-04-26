#ifndef SENSORPARSER_H
#define SENSORPARSER_H

#include <QSqlQuery>
#include <QPixmap>

class Sensor;
class SensorFrame;
class MeshModel;

class SensorParser
{
public:
    SensorParser();

    virtual void parseDBDataToSensor(const QSqlQuery &query, Sensor *sensor);
    virtual void parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor);
    virtual bool parseOnlineStatus4BytesDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor);

    virtual void updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor);
    virtual void updateSensorFromOnlineStatus(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor);
    virtual void updateSensorFromDB(MeshModel *meshModel, Sensor *dbSensor, Sensor *oldSensor);

    virtual void updateReferredSensorDataAndActionAfterDBUpdate(MeshModel *meshModel, Sensor *sensor);
    virtual void updateReferredSensorDataAndNotifyAfterOnlineStatusUpdate(MeshModel *meshModel, Sensor *sensor);
    virtual void updateReferredSensorDataAndActionAfterUserDataNotifyUpdate(MeshModel *meshModel, Sensor *sensor);

    virtual void parseSensorDataToSensor(const QString &sensorData, Sensor *sensor);
    virtual QString buildSensorDataFromSensor(Sensor *sensor);

    virtual QString getSensorTooltipInSensorListView(Sensor *sensor);
    QPixmap getSurfaceWithTouchInfo(MeshModel *meshModel, Sensor *sensor);
    virtual void parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame);

protected:
    SensorFrame *getSensorFrame();
    QPixmap buildSurfaceWithNoTouchInfo(MeshModel *meshModel, Sensor *sensor);

private:
    SensorFrame *m_sensorFrame;

};

#endif // SENSORPARSER_H
