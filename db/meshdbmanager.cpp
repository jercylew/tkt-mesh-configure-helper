#include "meshdbmanager.h"
#include "meshdbcaller.h"

MeshDBManager::MeshDBManager(const QString &meshUUID, QObject *parent) : QObject(parent)
{
    m_dbCaller=new MeshDbCaller(meshUUID);
}

MeshDBManager::~MeshDBManager()
{
    if(m_dbCaller!=NULL)
    {
        delete m_dbCaller;
        m_dbCaller=NULL;
    }
}

MeshDbCaller *MeshDBManager::dbCaller()
{
    return this->m_dbCaller;
}
