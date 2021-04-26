#include "meshruntimeparams.h"

MeshRuntimeParams::MeshRuntimeParams()
{
    m_cmdIntervalMsec=250;
    m_nodeOfflineDetectionDelaySec=5;
    m_checkIntervalSec=10;
    m_turnOffAutoNotificationWhenSchedulingIsStarted=true;
    m_autoRefreshNodesDuringMeshInitialization=false;
    m_thresholdOfBufferedCommandNumberInBusyScheduling=10;
    m_twoGroupsSchedulingTimeIntervalSec=3;
    m_ignoreMotionSensorHoldTime=false;
    m_luminaireMinBrightness=10;
}

int MeshRuntimeParams::cmdIntervalMsec()
{
    return this->m_cmdIntervalMsec;
}

void MeshRuntimeParams::setCmdIntervalMsec(int cmdIntervalMsec)
{
    this->m_cmdIntervalMsec=cmdIntervalMsec;
}

int MeshRuntimeParams::checkIntervalSec()
{
    return this->m_checkIntervalSec;
}

void MeshRuntimeParams::setCheckIntervalSec(int checkIntervalSec)
{
    this->m_checkIntervalSec=checkIntervalSec;
}

int MeshRuntimeParams::nodeOfflineDetectionDelaySec()
{
    return this->m_nodeOfflineDetectionDelaySec;
}

void MeshRuntimeParams::setNodeOfflineDetectionDelaySec(int nodeOfflineDetectionDelaySec)
{
    this->m_nodeOfflineDetectionDelaySec=nodeOfflineDetectionDelaySec;
}

bool MeshRuntimeParams::turnOffAutoNotificationWhenSchedulingIsStarted()
{
    return this->m_turnOffAutoNotificationWhenSchedulingIsStarted;
}

void MeshRuntimeParams::setTurnOffAutoNotificationWhenSchedulingIsStarted(bool turnOff)
{
    this->m_turnOffAutoNotificationWhenSchedulingIsStarted=turnOff;
}

bool MeshRuntimeParams::autoRefreshNodesDuringMeshInitialization()
{
    return this->m_autoRefreshNodesDuringMeshInitialization;
}

void MeshRuntimeParams::setAutoRefreshNodesDuringMeshInitialization(bool autoRefresh)
{
    this->m_autoRefreshNodesDuringMeshInitialization=autoRefresh;
}

int MeshRuntimeParams::thresholdOfBufferedCommandNumberInBusyScheduling()
{
    return this->m_thresholdOfBufferedCommandNumberInBusyScheduling;
}

void MeshRuntimeParams::setThresholdOfBufferedCommandNumberInBusyScheduling(int thresholdOfBufferedCommandNumberInBusyScheduling)
{
    this->m_thresholdOfBufferedCommandNumberInBusyScheduling=thresholdOfBufferedCommandNumberInBusyScheduling;
}

int MeshRuntimeParams::twoGroupsSchedulingTimeIntervalSec()
{
    return m_twoGroupsSchedulingTimeIntervalSec;
}

void MeshRuntimeParams::setTwoGroupsSchedulingTimeIntervalSec(int twoGroupsSchedulingTimeIntervalSec)
{
    m_twoGroupsSchedulingTimeIntervalSec = twoGroupsSchedulingTimeIntervalSec;
}

bool MeshRuntimeParams::ignoreMotionSensorHoldTime()
{
    return m_ignoreMotionSensorHoldTime;
}

void MeshRuntimeParams::setIgnoreMotionSensorHoldTime(bool ignoreMotionSensorHoldTime)
{
    m_ignoreMotionSensorHoldTime = ignoreMotionSensorHoldTime;
}

int MeshRuntimeParams::luminaireMinBrightness()
{
    return m_luminaireMinBrightness;
}

void MeshRuntimeParams::setLuminaireMinBrightness(int luminaireMinBrightness)
{
    m_luminaireMinBrightness = luminaireMinBrightness;
}

