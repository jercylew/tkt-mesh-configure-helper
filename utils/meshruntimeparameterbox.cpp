#include "meshruntimeparameterbox.h"

MeshRuntimeParameterBox::MeshRuntimeParameterBox(QObject *parent) : QObject(parent)
{
    m_executerOrderMethod=ExecuterOrderByNodeID;
    m_sensorOrderMethod=SensorOrderByNodeID;
    m_nodeGroupOrderMethod=NodeGroupOrderByGroupID;
    m_executerListViewCurrentGroupId=0;
    m_executerListOnlyShowOnlineNodes=false;
    m_executerListOnlyShowAvailableNodes=false;
    m_executerListViewScaleValue=1.0;
    m_sensorListViewScaleValue=1.0;
    m_sensorListOnlyShowAvailableNodes=false;
}

void MeshRuntimeParameterBox::setExecuterOrderMethod(MeshRuntimeParameterBox::ExecuterOrderMethod executerOrderMethod)
{
    this->m_executerOrderMethod=executerOrderMethod;
}

MeshRuntimeParameterBox::ExecuterOrderMethod MeshRuntimeParameterBox::executerOrderMethod()
{
    return this->m_executerOrderMethod;
}

void MeshRuntimeParameterBox::setSensorOrderMethod(MeshRuntimeParameterBox::SensorOrderMethod sensorOrderMethod)
{
    this->m_sensorOrderMethod=sensorOrderMethod;
}

MeshRuntimeParameterBox::SensorOrderMethod MeshRuntimeParameterBox::sensorOrderMethod()
{
    return this->m_sensorOrderMethod;
}

void MeshRuntimeParameterBox::setNodeGroupOrderMethod(MeshRuntimeParameterBox::NodeGroupOrderMethod nodeGroupOrderMethod)
{
    this->m_nodeGroupOrderMethod=nodeGroupOrderMethod;
}

MeshRuntimeParameterBox::NodeGroupOrderMethod MeshRuntimeParameterBox::nodeGroupOrderMethod()
{
    return this->m_nodeGroupOrderMethod;
}

void MeshRuntimeParameterBox::setExecuterListViewCurrentGroupId(quint8 groupId)
{
    this->m_executerListViewCurrentGroupId=groupId;
}

quint8 MeshRuntimeParameterBox::executerListViewCurrentGroupId()
{
    return this->m_executerListViewCurrentGroupId;
}

void MeshRuntimeParameterBox::setExecuterListOnlyShowOnlineNodes(bool onlyShowOnline)
{
    this->m_executerListOnlyShowOnlineNodes=onlyShowOnline;
}

bool MeshRuntimeParameterBox::executerListOnlyShowOnlineNodes()
{
    return this->m_executerListOnlyShowOnlineNodes;
}

void MeshRuntimeParameterBox::setExecuterListViewScaleValue(double scaleValue)
{
    this->m_executerListViewScaleValue=scaleValue;
}

double MeshRuntimeParameterBox::executerListViewScaleValue()
{
    return this->m_executerListViewScaleValue;
}

void MeshRuntimeParameterBox::setSensorListViewScaleValue(double scaleValue)
{
    this->m_sensorListViewScaleValue=scaleValue;
}

double MeshRuntimeParameterBox::sensorListViewScaleValue()
{
    return this->m_sensorListViewScaleValue;
}

bool MeshRuntimeParameterBox::executerListOnlyShowAvailableNodes()
{
    return m_executerListOnlyShowAvailableNodes;
}

void MeshRuntimeParameterBox::setExecuterListOnlyShowAvailableNodes(bool executerListOnlyShowAvailableNodes)
{
    m_executerListOnlyShowAvailableNodes = executerListOnlyShowAvailableNodes;
}

bool MeshRuntimeParameterBox::sensorListOnlyShowAvailableNodes()
{
    return m_sensorListOnlyShowAvailableNodes;
}

void MeshRuntimeParameterBox::setSensorListOnlyShowAvailableNodes(bool sensorListOnlyShowAvailableNodes)
{
    m_sensorListOnlyShowAvailableNodes = sensorListOnlyShowAvailableNodes;
}
