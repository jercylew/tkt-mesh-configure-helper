#ifndef SENSORSMODEL_H
#define SENSORSMODEL_H

#include <QObject>
#include "domain/sensor.h"
#include <QTimer>

class MeshModel;
class VirtualSwitchSensor;

class RealMotionSensor;
class LuxSensor;
class RealSwitchSensor;
class RealSwitchSensor2CH;
class RealMotionAndLuxCameraSensor;

class SensorsModel : public QObject
{
    Q_OBJECT
public:
    explicit SensorsModel(MeshModel *meshModel, QObject *parent = 0);

    int count();
    Sensor* at(int index);
    Sensor* getSensorBySensorId(const QString &sensorId);
    void setGroupList(quint8 addr, QList<quint8> groupList);

    QString getNodeName(const QString &sensorId);
    int getLuxValue(const QString &sensorId);
    bool contains(const QString sensorId);

    QList<Sensor *> getAll();
    QList<Sensor *> allRealMotionSensor();

    void addVirtualSensorToDB(Sensor *sensor);
    void updateVirtualSensorToDB(Sensor *sensor);
    void deleteVirtualSensor(const QString &sensorId);

    void clear();

    QList<VirtualSwitchSensor*> getBindingVirtualSwitchSensorListByRealSwitchSensorId(QString realSwitchSensorId);

    int sensorNumber(Sensor::SensorType sensorType);
    QList<Sensor::SensorType> realSensorTypeList();

    void updateRealMotionSensorToVirtualMotionSensorMap(const QString &virtualSensorId, const QList<QString> &realSensorIdList);

    void restartTimerWhenMotionSensorTriggered(const QString &sensorId);
    void onRealSwitchSensorTrig(const QString &realSwitchSensorId, quint8 realSwitchSensorBindGroupId, int trigValue);

    void executeRealMotionSensorSpecialLogicWhenUpdateReferredData(RealMotionSensor *realMotionSensor);
    void executeRealLuxSensorSpecialLogicWhenUpdateReferredData(LuxSensor *luxSensor);
    void executeRealSwitchSensorSpecialLogicWhenUpdateReferredData(RealSwitchSensor *realSwitchSensor);
    void executeRealSwitchSensorSpecialLogicWhenUpdateReferredData(RealSwitchSensor2CH *realSwitchSensor);
    void executeRealMotionLuxCameraSpecialLogicWhenUpdateReferredData(RealMotionAndLuxCameraSensor *realMotionLuxCamera);

    QList<Sensor::SensorType> allNumberableSensorTypeList();

    bool parseADataFrame(const QByteArray &dataFrame);

private:
    QList<Sensor *> sensorList;
    QMap<QString, Sensor *> sensorMap;
    QMap<QString, QTimer*> motionCheckedHoldingTimerMap;
    QMap<QString, QSet<QString>*> realMotionSensorIdToVirtualMotionSensorIdMap;
    QMap<Sensor::SensorType, int> m_sensorNumberMap;

    void updateSensorByDBReason(Sensor *dbSensor);
    void updateSensorByOnlineStatusReason(Sensor *sensor);
    void updateSensorByUserDataNotifyReason(Sensor *sensor);
    void updateReferredSensorDataAndActionAfterUserDataNotifyUpdate(Sensor *pSensor);

    MeshModel *m_meshModel;

    QTimer m_checkOnlineStatusChangeTimer;

    QList<Sensor::SensorType> m_realSensorTypeList;
    QList<Sensor::SensorType> m_virtualSensorTypeList;

    void updateSensorNumber(bool updateAll=true, Sensor *causeSensor=NULL);

private slots:
    void doMotionCheckedHoldingTimerTimeout();
    void doCheckOnlineStatusChange();

signals:
    void modelChanged();
    void dataChanged();
    void luxSensorNotify(const QString &sensorId, quint16 lux);
    void motionSensorNotify(const QString &sensorId);
    void sensorsNumChanged(Sensor::SensorType sensorType, int number);
    void updateAllSensorNumberDisplay();

public slots:
    void doSyncSensorNodes(QMap<QString, Sensor*> *sensorMap);
};

#endif // SENSORSMODEL_H
