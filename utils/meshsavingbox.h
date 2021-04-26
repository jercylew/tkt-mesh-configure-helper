#ifndef MESHSAVINGBOX_H
#define MESHSAVINGBOX_H

#include <QObject>
#include <QMap>
#include <QList>
#include "domain/meshruntimeparams.h"

class MeshRuntimeParameterBox;
class MeshRuntimeParams;

class MeshSavingBox : public QObject
{
    Q_OBJECT
public:
    explicit MeshSavingBox(const QString &meshUUID, QObject *parent = 0);
    void setValue(const QString &key, const QString &value);
    QString getValue(const QString &key);

    void setHide(QList<quint8> hideAddrs);
    bool isHide(quint8 addr);
    QList<quint8> getHideAddrs();

    void setGroupScheduleOn(quint8 groupId, bool on);
    bool isGroupScheduleOn(quint8 groupId);

    void setMeshRunTimeParams(MeshRuntimeParams meshRuntimeParams);
    MeshRuntimeParams meshRuntimeParams();

    int cmdIntervalMsec();
    int checkIntervalSec();
    int nodeOfflineDetectionDelaySec();
    bool turnOffAutoNotificationWhenSchedulingIsStarted();
    bool autoRefreshNodesDuringMeshInitialization();
    int thresholdOfBufferedCommandNumberInBusyScheduling();
    int twoGroupsSchedulingTimeIntervalSec();
    bool ignoreMotionSensorHoldTime();
    int luminaireMinBrightness();

    bool isHideLumWhenFoundSensor();
    void setHideLumWhenFoundSensor(bool ok);
    void loadHideLumWhenFoundSensorStatus();

    void setMeshBackendScheduleOn(bool on);
    bool isMeshBackendScheduleOn();

    void setEnableLogToDisk(bool enabled);
    bool enableLogToDisk();

    MeshRuntimeParameterBox *runtimeParameterBox();

    bool luxSensorShouldBeFiltered(const QString &luxSensorId);
    void setLuxSensorListThatShouldBeFiltered(QList<QString> luxSensorIdList);
    QList<QString> luxSensorListThatShouldBeFiltered();

    int luxSensorFilterCoefficient();
    void setLuxSensorFilterCoefficient(int coefficient);

    bool debugOutputIsOpen();
    void setDebugOutputIsOpen(bool debugOutputIsOpen);

private:
    QString m_meshUUID;
    QMap<quint8, bool> m_hideMap;
    QMap<QString, bool> m_filteredLuxSensorMap;
    QMap<quint8, bool> m_offGroupScheduleMap;
    bool m_hideLumWhenFoundSensor;
    MeshRuntimeParams m_meshRuntimeParams;
    bool m_backendScheduleOn;
    bool m_enableLogToDisk;
    int m_luxSensorFilterCoefficient;
    bool m_debugOutputIsOpen;

    void saveHide();
    void loadHide();
    void saveGroupScheduleStatus();
    void loadGroupScheduleStatus();
    void loadMeshRuntimeParams();
    void saveMeshScheduleStatus();
    void loadMeshScheduleStatus();
    void loadEnableLogToDiskStatus();
    void saveFilteredLuxSensor();
    void loadFilteredLuxSensor();
    void loadLuxSensorFilterCoefficient();

    MeshRuntimeParameterBox *m_runtimeParameterBox;

signals:

public slots:
};

#endif // MESHSAVINGBOX_H
