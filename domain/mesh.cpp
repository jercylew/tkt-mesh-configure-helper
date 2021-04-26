#include "mesh.h"

Mesh::Mesh()
{
    m_readFrames=0;
}

QString Mesh::meshUUID() const
{
    return m_meshUUID;
}

void Mesh::setMeshUUID(const QString &meshUUID)
{
    m_meshUUID = meshUUID;
}

QString Mesh::meshName() const
{
    return m_meshName;
}

void Mesh::setMeshName(const QString &meshName)
{
    m_meshName = meshName;
}

bool Mesh::autoScheduleIsOn() const
{
    return m_autoScheduleIsOn;
}

void Mesh::setAutoScheduleIsOn(bool autoScheduleIsOn)
{
    m_autoScheduleIsOn = autoScheduleIsOn;
}

QString Mesh::comName() const
{
    return m_comName;
}

void Mesh::setComName(const QString &comName)
{
    m_comName = comName;
}

bool Mesh::comIsOpened() const
{
    return m_comIsOpened;
}

void Mesh::setComIsOpened(bool comIsOpened)
{
    m_comIsOpened = comIsOpened;
}

bool Mesh::comIsEnabled() const
{
    return m_comIsEnabled;
}

void Mesh::setComIsEnabled(bool comIsEnabled)
{
    m_comIsEnabled = comIsEnabled;
}

void Mesh::load(Mesh *mesh)
{
    setMeshUUID(mesh->meshUUID());
    setMeshName(mesh->meshName());
    setAutoScheduleIsOn(mesh->autoScheduleIsOn());
    setComName(mesh->comName());
    setComIsEnabled(mesh->comIsEnabled());
    setComIsOpened(mesh->comIsOpened());
}

bool Mesh::bindComNameToMeshName() const
{
    return m_bindComNameToMeshName;
}

void Mesh::setBindComNameToMeshName(bool bindComNameToMeshName)
{
    m_bindComNameToMeshName = bindComNameToMeshName;
}

int Mesh::readFrames() const
{
    return m_readFrames;
}

void Mesh::setReadFrames(int readFrames)
{
    m_readFrames = readFrames;
}
