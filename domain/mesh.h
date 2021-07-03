#ifndef MESH_H
#define MESH_H

#include <QString>

class Mesh
{
public:
    Mesh();

    QString meshUUID() const;
    void setMeshUUID(const QString &meshUUID);

    QString meshName() const;
    void setMeshName(const QString &meshName);

    bool autoScheduleIsOn() const;
    void setAutoScheduleIsOn(bool autoScheduleIsOn);

    QString comName() const;
    void setComName(const QString &comName);

    bool comIsOpened() const;
    void setComIsOpened(bool comIsOpened);

    bool comIsEnabled() const;
    void setComIsEnabled(bool comIsEnabled);

    void load(Mesh *mesh);

    bool bindComNameToMeshName() const;
    void setBindComNameToMeshName(bool bindComNameToMeshName);

    int readFrames() const;
    void setReadFrames(int readFrames);

    quint8 gatewayMeshAddress() { return m_nGatewayAddress; }
    void setGatewayMeshAddress(quint8 nAddress) { m_nGatewayAddress = nAddress; }
    QString gatewayMeshName() { return m_strGatewayMeshName; }
    void setGatewayMeshName(QString strMeshName) { m_strGatewayMeshName = strMeshName; }
    QString gatewayMeshPassword() { return m_strGatewayMeshPassword; }
    void setGatewayMeshPassword(QString strMeshPassword) { m_strGatewayMeshPassword = strMeshPassword; }
    QString gatewayMeshLtk() { return m_strGatewayMeshLtk; }
    void setGatewayMeshLtk(QString strMeshLtk) { m_strGatewayMeshLtk = strMeshLtk; }

private:
    QString m_meshUUID;
    QString m_meshName;
    bool m_autoScheduleIsOn;
    QString m_comName;
    bool m_comIsOpened;
    bool m_comIsEnabled;
    bool m_bindComNameToMeshName;
    int m_readFrames;
    quint8 m_nGatewayAddress;
    QString m_strGatewayMeshName;
    QString m_strGatewayMeshLtk;
    QString m_strGatewayMeshPassword;
};

#endif // MESH_H
