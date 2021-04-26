#include "meshdevicenode.h"

MeshDeviceNode::MeshDeviceNode()
{

}

QString MeshDeviceNode::nodeId() const
{
    return m_nodeId;
}

void MeshDeviceNode::setNodeId(const QString &nodeId)
{
    m_nodeId = nodeId;
}

quint8 MeshDeviceNode::bluetoothAddress() const
{
    return m_bluetoothAddress;
}

void MeshDeviceNode::setBluetoothAddress(const quint8 &bluetoothAddress)
{
    m_bluetoothAddress = bluetoothAddress;
}

QString MeshDeviceNode::typeText() const
{
    return m_typeText;
}

void MeshDeviceNode::setTypeText(const QString &typeText)
{
    m_typeText = typeText;
}

bool MeshDeviceNode::online() const
{
    return m_online;
}

void MeshDeviceNode::setOnline(bool online)
{
    m_online = online;
}

QList<quint8> MeshDeviceNode::groupList() const
{
    return m_groupList;
}

void MeshDeviceNode::setGroupList(const QList<quint8> &groupList)
{
    m_groupList = groupList;
}

MeshDeviceNode *MeshDeviceNode::copy()
{
    MeshDeviceNode *node=new MeshDeviceNode;
    node->setNodeId(this->nodeId());
    node->setBluetoothAddress(this->bluetoothAddress());
    node->setOnline(this->online());
    node->setTypeText(this->typeText());
    node->setRegistered(this->registered());
    node->setGroupList(this->groupList());
    node->setDataText(this->dataText());
    return node;
}

void MeshDeviceNode::load(MeshDeviceNode *node)
{
    setBluetoothAddress(node->bluetoothAddress());
    setOnline(node->online());
    setTypeText(node->typeText());
    setRegistered(node->registered());
    setGroupList(node->groupList());
    setDataText(node->dataText());
}

bool MeshDeviceNode::registered() const
{
    return m_registered;
}

void MeshDeviceNode::setRegistered(bool registered)
{
    m_registered = registered;
}

QString MeshDeviceNode::dataText() const
{
    return m_dataText;
}

void MeshDeviceNode::setDataText(const QString &dataText)
{
    m_dataText = dataText;
}
