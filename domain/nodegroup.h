#ifndef NODEGROUP_H
#define NODEGROUP_H

#include <QMap>
#include "bluetoothnode.h"

class NodeGroup
{
public:
    typedef enum{
        ScheduleOff,
        ScheduleByDate,
        ScheduleByOnDuty,
        ScheduleByOffDuty
    }ScheduleType;

    typedef enum{
        LuminaireGroup,
        RelayGroup,
        ADBoardGroup
    }GroupType;

    NodeGroup();
    void addNode(BluetoothNode *node);
    void removeNode(quint8 nodeAddr);

    void setName(const QString &name);
    QString getName();
    void setId(quint8 id);
    quint8 getId();
    void setColor(const QString &color);
    QString getColor();
    int count();
    void setBrightness(quint8 brightness);
    quint8 getBrightness();
    void setScheduleType(ScheduleType scheduleType);
    ScheduleType getScheduleType();

    void loadParamsValueByGroupData(const QString &groupData);
    QString buildGroupData();

    static ScheduleType getScheduleTypeByText(const QString &scheduleTypeText);
    static QString getTextByScheduleType(ScheduleType scheduleType);
    static QList<QString> getScheduleTypeTextList();

    static GroupType getGroupTypeByText(const QString &groupTypeText);
    static QString getTextByGroupType(GroupType groupType);
    static QList<QString> getGroupTypeTextList();

    bool isScheduleOn();

    NodeGroup *copyAll();

    GroupType getGroupType() const;
    void setGroupType(const GroupType &groupType);

    bool hasSpecialControl();

private:
    QMap<quint8, BluetoothNode*> nodeMap;
    QString name;
    quint8 id;
    QString color;
    quint8 brightness;
    ScheduleType scheduleType;
    GroupType m_groupType;
};

#endif // NODEGROUP_H
