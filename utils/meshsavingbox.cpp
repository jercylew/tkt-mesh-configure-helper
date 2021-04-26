#include "meshsavingbox.h"

#include <QApplication>
#include <QSettings>
#include "globalsavingbox.h"
#include "meshruntimeparameterbox.h"

#define MAIN_CONFIG_FILE_PATH (QApplication::applicationDirPath()+"/run.ini")
#define HIDE_LUM_WHEN_FOUND_SENSOR "HIDE_LUM_WHEN_FOUND_SENSOR"

#define CMD_INTERVAL_MSEC   "CMD_INTERVAL_SEC"
#define CHECK_INTERVAL_SEC  "CHECK_INTERVAL_SEC"
#define NODE_OFFLINE_DETECTION_DELAY    "NODE_OFFLINE_DETECTION_DELAY"
#define MESH_BACKEND_SCHEDULE_ON    "MESH_BACKEND_SCHEDULE_ON"
#define TURN_OFF_AUTO_NOTIFICATION_WHEN_SCHEDULING_IS_STARTED   "TURN_OFF_AUTO_NOTIFICATION_WHEN_SCHEDULING_IS_STARTED"
#define AUTO_REFRESH_NODES_DURING_MESH_INITIALIZATION   "AUTO_REFRESH_NODES_DURING_MESH_INITIALIZATION"
#define THRESHOLD_OF_BUFFERED_COMMAND_NUMBER_IN_BUSY_SCHEDULING    "THRESHOLD_OF_BUFFERED_COMMAND_NUMBER_IN_BUSY_SCHEDULING"
#define TWO_GROUPS_SCHEDULING_TIME_INTERVAL_SEC "TWO_GROUPS_SCHEDULING_TIME_INTERVAL_SEC"
#define IGNORE_MOTION_SENSOR_HOLD_TIME  "IGNORE_MOTION_SENSOR_HOLD_TIME"
#define LUMINAIRE_MIN_BRIGHTNESS    "LUMINAIRE_MIN_BRIGHTNESS"

void MeshSavingBox::setValue(const QString &key, const QString &value)
{
    QSettings settings(GlobalSavingBox::getInstance()->getDataRootDirPath()+QString("/data/meshes/%1/run.ini").arg(m_meshUUID), QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.setValue(key, value);
    settings.sync();
}

QString MeshSavingBox::getValue(const QString &key)
{
    QSettings settings(GlobalSavingBox::getInstance()->getDataRootDirPath()+QString("/data/meshes/%1/run.ini").arg(m_meshUUID), QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    return settings.value(key).toString();
}

void MeshSavingBox::setHide(QList<quint8> hideAddrs)
{
    m_hideMap.clear();
    int len=hideAddrs.size();
    for(int i=0; i<len; i++)
    {
        m_hideMap.insert(hideAddrs.at(i), true);
    }
    saveHide();
}

bool MeshSavingBox::isHide(quint8 addr)
{
    if(m_hideMap.contains(addr))
    {
        return true;
    }
    else
    {
        return false;
    }
}

QList<quint8> MeshSavingBox::getHideAddrs()
{
    QList<quint8> hideAddrs=m_hideMap.keys();
    if(!hideAddrs.isEmpty())
    {
        qSort(hideAddrs.begin(), hideAddrs.end());
    }
    return hideAddrs;
}

void MeshSavingBox::setGroupScheduleOn(quint8 groupId, bool on)
{
    if(on)
    {
        m_offGroupScheduleMap.remove(groupId);
    }
    else
    {
        m_offGroupScheduleMap.insert(groupId, true);
    }
    saveGroupScheduleStatus();
}

bool MeshSavingBox::isGroupScheduleOn(quint8 groupId)
{
    return (!m_offGroupScheduleMap.contains(groupId));
}

void MeshSavingBox::setMeshRunTimeParams(MeshRuntimeParams meshRuntimeParams)
{
    this->m_meshRuntimeParams=meshRuntimeParams;
    this->setValue(CMD_INTERVAL_MSEC, QString::number(m_meshRuntimeParams.cmdIntervalMsec()));
    this->setValue(CHECK_INTERVAL_SEC, QString::number(m_meshRuntimeParams.checkIntervalSec()));
    this->setValue(NODE_OFFLINE_DETECTION_DELAY, QString::number(m_meshRuntimeParams.nodeOfflineDetectionDelaySec()));
    this->setValue(TURN_OFF_AUTO_NOTIFICATION_WHEN_SCHEDULING_IS_STARTED, m_meshRuntimeParams.turnOffAutoNotificationWhenSchedulingIsStarted()?"true":"false");
    this->setValue(AUTO_REFRESH_NODES_DURING_MESH_INITIALIZATION, m_meshRuntimeParams.autoRefreshNodesDuringMeshInitialization()?"true":"false");
    this->setValue(THRESHOLD_OF_BUFFERED_COMMAND_NUMBER_IN_BUSY_SCHEDULING, QString::number(m_meshRuntimeParams.thresholdOfBufferedCommandNumberInBusyScheduling()));
    this->setValue(TWO_GROUPS_SCHEDULING_TIME_INTERVAL_SEC, QString::number(meshRuntimeParams.twoGroupsSchedulingTimeIntervalSec()));
    this->setValue(IGNORE_MOTION_SENSOR_HOLD_TIME, m_meshRuntimeParams.ignoreMotionSensorHoldTime()?"true":"false");
    this->setValue(LUMINAIRE_MIN_BRIGHTNESS, QString::number(m_meshRuntimeParams.luminaireMinBrightness()));
}

MeshRuntimeParams MeshSavingBox::meshRuntimeParams()
{
    return this->m_meshRuntimeParams;
}

int MeshSavingBox::cmdIntervalMsec()
{
    return this->m_meshRuntimeParams.cmdIntervalMsec();
}

int MeshSavingBox::checkIntervalSec()
{
    return this->m_meshRuntimeParams.checkIntervalSec();
}

int MeshSavingBox::nodeOfflineDetectionDelaySec()
{
    return this->m_meshRuntimeParams.nodeOfflineDetectionDelaySec();
}

bool MeshSavingBox::turnOffAutoNotificationWhenSchedulingIsStarted()
{
    return this->m_meshRuntimeParams.turnOffAutoNotificationWhenSchedulingIsStarted();
}

bool MeshSavingBox::autoRefreshNodesDuringMeshInitialization()
{
    return m_meshRuntimeParams.autoRefreshNodesDuringMeshInitialization();
}

int MeshSavingBox::thresholdOfBufferedCommandNumberInBusyScheduling()
{
    return m_meshRuntimeParams.thresholdOfBufferedCommandNumberInBusyScheduling();
}

int MeshSavingBox::twoGroupsSchedulingTimeIntervalSec()
{
    return m_meshRuntimeParams.twoGroupsSchedulingTimeIntervalSec();
}

bool MeshSavingBox::ignoreMotionSensorHoldTime()
{
    return m_meshRuntimeParams.ignoreMotionSensorHoldTime();
}

bool MeshSavingBox::isHideLumWhenFoundSensor()
{
    return m_hideLumWhenFoundSensor;
}

void MeshSavingBox::setHideLumWhenFoundSensor(bool ok)
{
    this->m_hideLumWhenFoundSensor=ok;
    this->setValue(HIDE_LUM_WHEN_FOUND_SENSOR, ok?QString("true"):QString("false"));
}

void MeshSavingBox::loadHideLumWhenFoundSensorStatus()
{
    if(this->getValue(HIDE_LUM_WHEN_FOUND_SENSOR)=="false")
    {
        m_hideLumWhenFoundSensor=false;
    }
    else
    {
        m_hideLumWhenFoundSensor=true;
    }
}

void MeshSavingBox::setMeshBackendScheduleOn(bool on)
{
    this->m_backendScheduleOn=on;
    saveMeshScheduleStatus();
}

bool MeshSavingBox::isMeshBackendScheduleOn()
{
    return this->m_backendScheduleOn;
}

void MeshSavingBox::setEnableLogToDisk(bool enabled)
{
    this->m_enableLogToDisk=enabled;
    setValue("ENABLE_LOG_TO_DISK", enabled?"true":"false");
}

bool MeshSavingBox::enableLogToDisk()
{
    return this->m_enableLogToDisk;
}

MeshRuntimeParameterBox *MeshSavingBox::runtimeParameterBox()
{
    return m_runtimeParameterBox;
}

bool MeshSavingBox::luxSensorShouldBeFiltered(const QString &luxSensorId)
{
    return m_filteredLuxSensorMap.contains(luxSensorId);
}

void MeshSavingBox::setLuxSensorListThatShouldBeFiltered(QList<QString> luxSensorIdList)
{
    m_filteredLuxSensorMap.clear();
    int len=luxSensorIdList.size();
    for(int i=0; i<len; i++)
    {
        m_filteredLuxSensorMap.insert(luxSensorIdList.at(i), true);
    }
    saveFilteredLuxSensor();
}

QList<QString> MeshSavingBox::luxSensorListThatShouldBeFiltered()
{
    QList<QString> hideAddrs=m_filteredLuxSensorMap.keys();
    if(!hideAddrs.isEmpty())
    {
        qSort(hideAddrs.begin(), hideAddrs.end());
    }
    return hideAddrs;
}

int MeshSavingBox::luxSensorFilterCoefficient()
{
    if(m_luxSensorFilterCoefficient<3)
    {
        return 3;
    }
    else if(m_luxSensorFilterCoefficient>51)
    {
        return 51;
    }
    return this->m_luxSensorFilterCoefficient;
}

void MeshSavingBox::setLuxSensorFilterCoefficient(int coefficient)
{
    this->m_luxSensorFilterCoefficient=coefficient;
    setValue("LUX_SENSOR_FILTER_COEFFICIENT", QString::number(this->m_luxSensorFilterCoefficient));
}

bool MeshSavingBox::debugOutputIsOpen()
{
    return m_debugOutputIsOpen;
}

void MeshSavingBox::setDebugOutputIsOpen(bool debugOutputIsOpen)
{
    m_debugOutputIsOpen = debugOutputIsOpen;
}

int MeshSavingBox::luminaireMinBrightness()
{
    return m_meshRuntimeParams.luminaireMinBrightness();
}

void MeshSavingBox::saveHide()
{
    QList<quint8> keys=m_hideMap.keys();
    QString str;
    int len=keys.size();
    for(int i=0; i<len; i++)
    {
        str.push_back(QString("%1,").arg(keys.at(i)));
    }
    setValue("HIDE_ADDRS_HEX", QString::fromLatin1(str.toLatin1().toBase64()));
}

void MeshSavingBox::loadHide()
{
    m_hideMap.clear();
    QString hex=getValue("HIDE_ADDRS_HEX");
    if(hex.trimmed().isEmpty())
    {
        return;
    }
    QString str=QString::fromLatin1(QByteArray::fromBase64(hex.toLatin1()));
    QList<QString> strs=str.split(",");
    int len=strs.length();
    for(int i=0; i<len; i++)
    {
        if(strs.at(i).trimmed().isEmpty())
        {
            continue;
        }
        m_hideMap.insert((quint8)(strs.at(i).trimmed().toInt()), true);
    }
}

void MeshSavingBox::saveGroupScheduleStatus()
{
    QList<quint8> keys=m_offGroupScheduleMap.keys();
    QString str;
    int len=keys.size();
    for(int i=0; i<len; i++)
    {
        str.push_back(QString("%1,").arg(keys.at(i)));
    }
    setValue("OFF_SCHEDULE_GROUPS_HEX", QString::fromLatin1(str.toLatin1().toBase64()));
}

void MeshSavingBox::loadGroupScheduleStatus()
{
    m_offGroupScheduleMap.clear();
    QString hex=getValue("OFF_SCHEDULE_GROUPS_HEX");
    if(hex.trimmed().isEmpty())
    {
        return;
    }
    QString str=QString::fromLatin1(QByteArray::fromBase64(hex.toLatin1()));
    QList<QString> strs=str.split(",");
    int len=strs.length();
    for(int i=0; i<len; i++)
    {
        if(strs.at(i).trimmed().isEmpty())
        {
            continue;
        }
        m_offGroupScheduleMap.insert((quint8)(strs.at(i).trimmed().toInt()), true);
    }
}

void MeshSavingBox::loadMeshRuntimeParams()
{
    int cmdIntervalMsec=this->getValue(CMD_INTERVAL_MSEC).toInt();
    if(cmdIntervalMsec<=0)
    {
        cmdIntervalMsec=250;
    }
    else if(cmdIntervalMsec<200)
    {
        cmdIntervalMsec=200;
    }
    else if(cmdIntervalMsec>1000)
    {
        cmdIntervalMsec=1000;
    }

    int checkIntervalSec=this->getValue(CHECK_INTERVAL_SEC).toInt();
    if(checkIntervalSec<=0)
    {
        checkIntervalSec=10;
    }
    else if(checkIntervalSec<2)
    {
        checkIntervalSec=2;
    }
    else if(checkIntervalSec>10)
    {
        checkIntervalSec=10;
    }

    int nodeOfflineDetectionDelaySec=this->getValue(NODE_OFFLINE_DETECTION_DELAY).toInt();
    if(nodeOfflineDetectionDelaySec<=0)
    {
        nodeOfflineDetectionDelaySec=0;
    }
    else if(nodeOfflineDetectionDelaySec>=60)
    {
        nodeOfflineDetectionDelaySec=60;
    }

    bool turnOffAutoNotificationWhenShedulingIsStarted=((this->getValue(TURN_OFF_AUTO_NOTIFICATION_WHEN_SCHEDULING_IS_STARTED)=="false")?false:true);
    bool autoRefreshNodesDuringMeshInitialization=((this->getValue(AUTO_REFRESH_NODES_DURING_MESH_INITIALIZATION)=="true")?true:false);

    int thresholdOfCommandNumberInBusyScheduling=this->getValue(THRESHOLD_OF_BUFFERED_COMMAND_NUMBER_IN_BUSY_SCHEDULING).toInt();
    if(thresholdOfCommandNumberInBusyScheduling<=0)
    {
        thresholdOfCommandNumberInBusyScheduling=20;
    }
    else if(thresholdOfCommandNumberInBusyScheduling>=500)
    {
        thresholdOfCommandNumberInBusyScheduling=500;
    }

    int twoGroupsSchedulingTimeIntervalSec=this->getValue(TWO_GROUPS_SCHEDULING_TIME_INTERVAL_SEC).toInt();
    if(twoGroupsSchedulingTimeIntervalSec<=0)
    {
        twoGroupsSchedulingTimeIntervalSec=0;
    }
    else if(twoGroupsSchedulingTimeIntervalSec>=10)
    {
        twoGroupsSchedulingTimeIntervalSec=10;
    }

    bool ignoreMotionSensorHoldTime=(this->getValue(IGNORE_MOTION_SENSOR_HOLD_TIME)=="true")?true:false;

    int luminaireMinBrightness=getValue(LUMINAIRE_MIN_BRIGHTNESS).toInt();
    if(luminaireMinBrightness<=0)
    {
        luminaireMinBrightness=10;
    }
    else if(luminaireMinBrightness<5)
    {
        luminaireMinBrightness=5;
    }
    else if(luminaireMinBrightness>50)
    {
        luminaireMinBrightness=50;
    }

    MeshRuntimeParams params;
    params.setCmdIntervalMsec(cmdIntervalMsec);
    params.setCheckIntervalSec(checkIntervalSec);
    params.setNodeOfflineDetectionDelaySec(nodeOfflineDetectionDelaySec);
    params.setTurnOffAutoNotificationWhenSchedulingIsStarted(turnOffAutoNotificationWhenShedulingIsStarted);
    params.setAutoRefreshNodesDuringMeshInitialization(autoRefreshNodesDuringMeshInitialization);
    params.setThresholdOfBufferedCommandNumberInBusyScheduling(thresholdOfCommandNumberInBusyScheduling);
    params.setTwoGroupsSchedulingTimeIntervalSec(twoGroupsSchedulingTimeIntervalSec);
    params.setIgnoreMotionSensorHoldTime(ignoreMotionSensorHoldTime);
    params.setLuminaireMinBrightness(luminaireMinBrightness);
    setMeshRunTimeParams(params);
}

void MeshSavingBox::saveMeshScheduleStatus()
{
    if(m_backendScheduleOn)
    {
        this->setValue(MESH_BACKEND_SCHEDULE_ON, "true");
    }
    else
    {
        this->setValue(MESH_BACKEND_SCHEDULE_ON, "false");
    }
}

void MeshSavingBox::loadMeshScheduleStatus()
{
    QString on=this->getValue(MESH_BACKEND_SCHEDULE_ON);
    if(on=="true")
    {
        m_backendScheduleOn=true;
    }
    else
    {
        m_backendScheduleOn=false;
    }
}

void MeshSavingBox::loadEnableLogToDiskStatus()
{
    QString enabled=getValue("ENABLE_LOG_TO_DISK");
    if(enabled=="false")
    {
        m_enableLogToDisk=false;
    }
    else
    {
        m_enableLogToDisk=true;
    }
}

void MeshSavingBox::saveFilteredLuxSensor()
{
    QList<QString> keys=m_filteredLuxSensorMap.keys();
    QString str;
    int len=keys.size();
    for(int i=0; i<len; i++)
    {
        str.push_back(QString("%1,").arg(keys.at(i)));
    }
    setValue("FILTERED_LUX_SENSOR_LIST_BASE64", QString::fromLatin1(str.toLatin1().toBase64()));
}

void MeshSavingBox::loadFilteredLuxSensor()
{
    m_filteredLuxSensorMap.clear();
    QString base64=getValue("FILTERED_LUX_SENSOR_LIST_BASE64");
    if(base64.trimmed().isEmpty())
    {
        return;
    }
    QString str=QString::fromLatin1(QByteArray::fromBase64(base64.toLatin1()));
    QList<QString> strs=str.split(",");
    int len=strs.length();
    for(int i=0; i<len; i++)
    {
        if(strs.at(i).trimmed().isEmpty())
        {
            continue;
        }
        m_filteredLuxSensorMap.insert(strs.at(i).trimmed(), true);
    }
}

void MeshSavingBox::loadLuxSensorFilterCoefficient()
{
    m_luxSensorFilterCoefficient=getValue("LUX_SENSOR_FILTER_COEFFICIENT").toInt();
    if(m_luxSensorFilterCoefficient<3)
    {
        m_luxSensorFilterCoefficient=3;
    }
    else if(m_luxSensorFilterCoefficient>51)
    {
        m_luxSensorFilterCoefficient=51;
    }
}

MeshSavingBox::MeshSavingBox(const QString &meshUUID, QObject *parent)
    : QObject(parent),
      m_meshUUID(meshUUID)
{
    loadHide();
    loadGroupScheduleStatus();
    loadHideLumWhenFoundSensorStatus();
    loadMeshRuntimeParams();
    loadMeshScheduleStatus();
    loadEnableLogToDiskStatus();
    loadFilteredLuxSensor();
    loadLuxSensorFilterCoefficient();

    m_runtimeParameterBox=new MeshRuntimeParameterBox(this);
    m_debugOutputIsOpen=false;
}
