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

private:
    QString m_meshUUID;
    QString m_meshName;
    bool m_autoScheduleIsOn;
    QString m_comName;
    bool m_comIsOpened;
    bool m_comIsEnabled;
    bool m_bindComNameToMeshName;
    int m_readFrames;
};

#endif // MESH_H
