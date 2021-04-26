#include "tktmesh.h"

TKTMesh::TKTMesh()
{
    this->setInCloudConfigMode(false);
}

QString TKTMesh::hostID() const
{
    return m_hostID;
}

void TKTMesh::setHostID(const QString &hostID)
{
    m_hostID = hostID;
}

QString TKTMesh::hostKey() const
{
    return m_hostKey;
}

void TKTMesh::setHostKey(const QString &hostKey)
{
    m_hostKey = hostKey;
}

QString TKTMesh::hostName() const
{
    return m_hostName;
}

void TKTMesh::setHostName(const QString &hostName)
{
    m_hostName = hostName;
}

QString TKTMesh::hostDescription() const
{
    return m_hostDescription;
}

void TKTMesh::setHostDescription(const QString &hostDescription)
{
    m_hostDescription = hostDescription;
}

QString TKTMesh::hostIP() const
{
    return m_hostIP;
}

void TKTMesh::setHostIP(const QString &hostIP)
{
    m_hostIP = hostIP;
}

QString TKTMesh::hostPeerIP() const
{
    return m_hostPeerIP;
}

void TKTMesh::setHostPeerIP(const QString &hostPeerIP)
{
    m_hostPeerIP = hostPeerIP;
}

QString TKTMesh::hostHardVersion() const
{
    return m_hostHardVersion;
}

void TKTMesh::setHostHardVersion(const QString &hostHardVersion)
{
    m_hostHardVersion = hostHardVersion;
}

QString TKTMesh::hostSoftVersion() const
{
    return m_hostSoftVersion;
}

void TKTMesh::setHostSoftVersion(const QString &hostSoftVersion)
{
    m_hostSoftVersion = hostSoftVersion;
}

void TKTMesh::load(TKTMesh *tktMesh)
{
    setHostID(tktMesh->hostID());
    setHostCode(tktMesh->hostCode());
    setHostKey(tktMesh->hostKey());
    setHostName(tktMesh->hostName());
    setHostDescription(tktMesh->hostDescription());
    setHostIP(tktMesh->hostIP());
    setHostPeerIP(tktMesh->hostPeerIP());
    setHostHardVersion(tktMesh->hostHardVersion());
    setHostSoftVersion(tktMesh->hostSoftVersion());
    setHostRegistered(tktMesh->hostRegistered());
    setPlatformAddress(tktMesh->platformAddress());
    setPlatformPort(tktMesh->platformPort());
    setSystemTime(tktMesh->systemTime());
    setUpdateDescription(tktMesh->updateDescription());
    setPlatform(tktMesh->platform());
}

TKTMesh *TKTMesh::copy()
{
    TKTMesh *tktMesh=new TKTMesh;
    tktMesh->setHostID(this->m_hostID);
    tktMesh->setHostCode(this->m_hostCode);
    tktMesh->setHostKey(this->m_hostKey);
    tktMesh->setHostName(this->m_hostName);
    tktMesh->setHostDescription(this->m_hostDescription);
    tktMesh->setHostIP(this->m_hostIP);
    tktMesh->setHostPeerIP(this->m_hostPeerIP);
    tktMesh->setHostHardVersion(this->m_hostHardVersion);
    tktMesh->setHostSoftVersion(this->m_hostSoftVersion);
    tktMesh->setHostRegistered(this->m_hostRegistered);
    tktMesh->setPlatformAddress(this->platformAddress());
    tktMesh->setPlatformPort(this->platformPort());
    tktMesh->setSystemTime(this->systemTime());
    tktMesh->setPlatform(this->platform());
    return tktMesh;
}

bool TKTMesh::hostRegistered() const
{
    return m_hostRegistered;
}

void TKTMesh::setHostRegistered(bool hostRegistered)
{
    m_hostRegistered = hostRegistered;
}

QString TKTMesh::platformAddress() const
{
    return m_platformAddress;
}

void TKTMesh::setPlatformAddress(const QString &platformAddress)
{
    m_platformAddress = platformAddress;
}

int TKTMesh::platformPort() const
{
    return m_platformPort;
}

void TKTMesh::setPlatformPort(int platformPort)
{
    m_platformPort = platformPort;
}

QString TKTMesh::hostCode() const
{
    return m_hostCode;
}

void TKTMesh::setHostCode(const QString &hostCode)
{
    m_hostCode = hostCode;
}

QString TKTMesh::systemTime() const
{
    return m_systemTime;
}

void TKTMesh::setSystemTime(const QString &systemTime)
{
    m_systemTime = systemTime;
}

QString TKTMesh::updateDescription() const
{
    return m_updateDescription;
}

void TKTMesh::setUpdateDescription(const QString &updateDescription)
{
    m_updateDescription = updateDescription;
}

QString TKTMesh::updateProgress() const
{
    return m_updateProgress;
}

void TKTMesh::setUpdateProgress(const QString &updateProgress)
{
    m_updateProgress = updateProgress;
}

QString TKTMesh::platform() const
{
    return m_platform;
}

void TKTMesh::setPlatform(const QString &platform)
{
    m_platform = platform;
}

QString TKTMesh::cloudConfigAddress() const
{
    return m_cloudConfigAddress;
}

void TKTMesh::setCloudConfigAddress(const QString &cloudConfigAddress)
{
    m_cloudConfigAddress = cloudConfigAddress;
}

int TKTMesh::cloudConfigPort() const
{
    return m_cloudConfigPort;
}

void TKTMesh::setCloudConfigPort(int cloudConfigPort)
{
    m_cloudConfigPort = cloudConfigPort;
}

bool TKTMesh::inCloudConfigMode() const
{
    return m_inCloudConfigMode;
}

void TKTMesh::setInCloudConfigMode(bool inCloudConfigMode)
{
    m_inCloudConfigMode = inCloudConfigMode;
}

int TKTMesh::cloudConfigConfigerPort() const
{
    return m_cloudConfigConfigerPort;
}

void TKTMesh::setCloudConfigConfigerPort(int cloudConfigConfigerPort)
{
    m_cloudConfigConfigerPort = cloudConfigConfigerPort;
}

int TKTMesh::dbDataPackageMaxSizeInKB() const
{
    if((m_dbDataPackageMaxSizeInKB<1) || (m_dbDataPackageMaxSizeInKB>100))
    {
        return 10;
    }
    return m_dbDataPackageMaxSizeInKB;
}

void TKTMesh::setDbDataPackageMaxSizeInKB(int dbDataPackageMaxSizeInKB)
{
    m_dbDataPackageMaxSizeInKB = dbDataPackageMaxSizeInKB;
}
