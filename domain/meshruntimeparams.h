#ifndef RUNTIMEPARAMS_H
#define RUNTIMEPARAMS_H


class MeshRuntimeParams
{
public:
    MeshRuntimeParams();

    int cmdIntervalMsec();
    void setCmdIntervalMsec(int cmdIntervalMsec);

    int checkIntervalSec();
    void setCheckIntervalSec(int checkIntervalSec);

    int nodeOfflineDetectionDelaySec();
    void setNodeOfflineDetectionDelaySec(int nodeOfflineDetectionDelaySec);

    bool turnOffAutoNotificationWhenSchedulingIsStarted();
    void setTurnOffAutoNotificationWhenSchedulingIsStarted(bool turnOff);

    bool autoRefreshNodesDuringMeshInitialization();
    void setAutoRefreshNodesDuringMeshInitialization(bool autoRefresh);

    int thresholdOfBufferedCommandNumberInBusyScheduling();
    void setThresholdOfBufferedCommandNumberInBusyScheduling(int thresholdOfBufferedCommandNumberInBusyScheduling);

    int twoGroupsSchedulingTimeIntervalSec();
    void setTwoGroupsSchedulingTimeIntervalSec(int twoGroupsSchedulingTimeIntervalSec);

    bool ignoreMotionSensorHoldTime();
    void setIgnoreMotionSensorHoldTime(bool ignoreMotionSensorHoldTime);

    int luminaireMinBrightness();
    void setLuminaireMinBrightness(int luminaireMinBrightness);

private:
    int m_cmdIntervalMsec;
    int m_checkIntervalSec;
    int m_nodeOfflineDetectionDelaySec;
    bool m_turnOffAutoNotificationWhenSchedulingIsStarted;
    bool m_autoRefreshNodesDuringMeshInitialization;
    int m_thresholdOfBufferedCommandNumberInBusyScheduling;
    int m_twoGroupsSchedulingTimeIntervalSec;
    bool m_ignoreMotionSensorHoldTime;
    int m_luminaireMinBrightness;
};

#endif // RUNTIMEPARAMS_H
