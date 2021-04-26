#ifndef BLUETOOTHNODE_H
#define BLUETOOTHNODE_H

#include <QString>
#include <QMap>
#include <QDateTime>
#include <QDebug>

class BluetoothNode
{
public:
    BluetoothNode();

    void setName(const QString &name);

    QString getName();

    void setBluetoothAddress(quint8 address);

    quint8 getBluetoothAddress();

    void setOnline(bool online, int delayMSec=0);

    void checkOnlineStatusTimeoutChange(int delaySec=0);

    bool isOnline();

    QList<quint8> getGroupList();

    void setGroupList(QList<quint8> groupList);

    bool belongsToGroup(quint8 groupId);

    int groupListSize();

    virtual QString typeText()=0;

    virtual QString displayEnglishTypeText();

    virtual QString shortTypeText()=0;

    virtual QString nodeId()=0;

    virtual QString dataText();

    virtual QString nodeKindText()=0;

    bool isTouched();
    void setTouched(bool touched);

    virtual bool isConfigurable();
    virtual bool isVirtual();

    virtual bool hasSpecialControl();

private:
    QString name;
    quint8 bluetoothAddress;
    bool online;
    QList<quint8> groupList;
    long long lastSetOnlineOnMSecs;
    bool isGoingToOfflineWhenTimeout;
    bool m_touched;
};

#endif // BLUETOOTHNODE_H
