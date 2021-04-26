#ifndef MESHDEVICENODE_H
#define MESHDEVICENODE_H

#include <QString>
#include <QList>

class MeshDeviceNode
{
public:
    MeshDeviceNode();

    QString nodeId() const;
    void setNodeId(const QString &nodeId);

    quint8 bluetoothAddress() const;
    void setBluetoothAddress(const quint8 &bluetoothAddress);

    QString typeText() const;
    void setTypeText(const QString &typeText);

    bool online() const;
    void setOnline(bool online);

    QList<quint8> groupList() const;
    void setGroupList(const QList<quint8> &groupList);

    MeshDeviceNode *copy();
    void load(MeshDeviceNode *node);

    bool registered() const;
    void setRegistered(bool registered);

    QString dataText() const;
    void setDataText(const QString &dataText);

private:
    QString m_nodeId;
    quint8 m_bluetoothAddress;
    QString m_typeText;
    bool m_online;
    bool m_registered;
    QList<quint8> m_groupList;
    QString m_dataText;
};

#endif // MESHDEVICENODE_H
