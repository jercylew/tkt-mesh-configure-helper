#include "remoteconfigmanager.h"
#include "utils/remoteconfigsessionholder.h"

RemoteConfigManager::RemoteConfigManager(PrepareMeshModel *prepareMeshModel, QObject *parent) : QObject(parent)
{
    this->m_prepareMeshModel=prepareMeshModel;
    m_remoteConfigSessionHolder=new RemoteConfigSessionHolder(prepareMeshModel, this);
}

RemoteConfigSessionHolder *RemoteConfigManager::remoteConfigSessionHolder()
{
    return m_remoteConfigSessionHolder;
}
