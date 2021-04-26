#include "bluetoothnode.h"

//60 seconds
#define NODE_LAZY_OFFLINE_DELAY_SEC     60

BluetoothNode::BluetoothNode()
{
    lastSetOnlineOnMSecs=0;
    isGoingToOfflineWhenTimeout=false;
    m_touched=false;
    bluetoothAddress=0;
    this->online=false;
}

void BluetoothNode::setName(const QString &name)
{
    this->name=name;
}

QString BluetoothNode::getName()
{
    return this->name;
}

void BluetoothNode::setBluetoothAddress(quint8 address)
{
    this->bluetoothAddress=address;
}

quint8 BluetoothNode::getBluetoothAddress()
{
    return this->bluetoothAddress;
}

void BluetoothNode::setOnline(bool online, int delayMSec)
{
    long long currentMsec=QDateTime::currentMSecsSinceEpoch();
    if(online)
    {
        this->online=true;
        lastSetOnlineOnMSecs=currentMsec;
        isGoingToOfflineWhenTimeout=false;
    }
    else
    {
        if(this->online==true)
        {
            if((currentMsec-lastSetOnlineOnMSecs)<delayMSec)
            {
                isGoingToOfflineWhenTimeout=true;
            }
            else
            {
                this->online=false;
                isGoingToOfflineWhenTimeout=false;
            }
        }
        else
        {
            isGoingToOfflineWhenTimeout=false;
        }
    }
}

void BluetoothNode::checkOnlineStatusTimeoutChange(int delayMSec)
{
    if(!isGoingToOfflineWhenTimeout)
    {
        return;
    }
    long long currentMsec=QDateTime::currentMSecsSinceEpoch();
    if((currentMsec-lastSetOnlineOnMSecs)<delayMSec)
    {
        return;
    }
    online=false;
    isGoingToOfflineWhenTimeout=false;
}

bool BluetoothNode::isOnline()
{
    return this->online;
}

QList<quint8> BluetoothNode::getGroupList()
{
    return groupList;
}

void BluetoothNode::setGroupList(QList<quint8> groupList)
{
    this->groupList=groupList;
}

bool BluetoothNode::belongsToGroup(quint8 groupId)
{
    return groupList.contains(groupId);
}

int BluetoothNode::groupListSize()
{
    return groupList.size();
}

QString BluetoothNode::displayEnglishTypeText()
{
    return typeText();
}

QString BluetoothNode::dataText()
{
    return "";
}

bool BluetoothNode::isTouched()
{
    return this->m_touched;
}

void BluetoothNode::setTouched(bool touched)
{
    this->m_touched=touched;
}

/**
 * 用于标记是否可以被配置，用在是否显示“配置”菜单项等地方
 * 需要注意的是在本系统中所有的节点类型都是可配置的，本函数只用来控制菜单显示不能用来控制具体业务逻辑
 */
bool BluetoothNode::isConfigurable()
{
    return false;
}

/**
 * 用于标记是否是虚拟设备，用在是否显示附加小图标等地方
 */
bool BluetoothNode::isVirtual()
{
    return false;
}

bool BluetoothNode::hasSpecialControl()
{
    return false;
}

