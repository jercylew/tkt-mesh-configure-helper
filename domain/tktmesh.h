#ifndef TKTMESH_H
#define TKTMESH_H

#include <QString>

class TKTMesh
{
public:
    TKTMesh();

    QString hostID() const;
    void setHostID(const QString &hostID);

    QString hostKey() const;
    void setHostKey(const QString &hostKey);

    QString hostName() const;
    void setHostName(const QString &hostName);

    QString hostDescription() const;
    void setHostDescription(const QString &hostDescription);

    QString hostIP() const;
    void setHostIP(const QString &hostIP);

    QString hostPeerIP() const;
    void setHostPeerIP(const QString &hostPeerIP);

    QString hostHardVersion() const;
    void setHostHardVersion(const QString &hostHardVersion);

    QString hostSoftVersion() const;
    void setHostSoftVersion(const QString &hostSoftVersion);

    void load(TKTMesh *tktMesh);
    TKTMesh *copy();

    bool hostRegistered() const;
    void setHostRegistered(bool hostRegistered);

    QString platformAddress() const;
    void setPlatformAddress(const QString &platformAddress);

    int platformPort() const;
    void setPlatformPort(int platformPort);

    QString hostCode() const;
    void setHostCode(const QString &hostCode);

    QString systemTime() const;
    void setSystemTime(const QString &systemTime);

    QString updateDescription() const;
    void setUpdateDescription(const QString &updateDescription);

    QString updateProgress() const;
    void setUpdateProgress(const QString &updateProgress);

    QString platform() const;
    void setPlatform(const QString &platform);

    QString cloudConfigAddress() const;
    void setCloudConfigAddress(const QString &cloudConfigAddress);

    int cloudConfigPort() const;
    void setCloudConfigPort(int cloudConfigPort);

    bool inCloudConfigMode() const;
    void setInCloudConfigMode(bool inCloudConfigMode);

    int cloudConfigConfigerPort() const;
    void setCloudConfigConfigerPort(int cloudConfigConfigerPort);

    int dbDataPackageMaxSizeInKB() const;
    void setDbDataPackageMaxSizeInKB(int dbDataPackageMaxSizeInKB);

private:
    QString m_hostID;
    QString m_hostCode;
    QString m_hostName;
    QString m_hostDescription;
    QString m_hostKey;
    QString m_hostIP;
    QString m_hostPeerIP;
    QString m_hostHardVersion;
    QString m_hostSoftVersion;
    bool m_hostRegistered;
    QString m_platformAddress;
    int m_platformPort;
    QString m_cloudConfigAddress;
    int m_cloudConfigPort;
    int m_cloudConfigConfigerPort;
    QString m_systemTime;
    QString m_updateDescription;
    QString m_updateProgress;
    QString m_platform;
    bool m_inCloudConfigMode;
    int m_dbDataPackageMaxSizeInKB;


};

#endif // TKTMESH_H
