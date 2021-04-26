#include "nodegroup.h"
#include "utils/tools.h"
#include <QJsonObject>

NodeGroup::NodeGroup()
{
    brightness=-1;
    scheduleType=ScheduleByDate;
    m_groupType=LuminaireGroup;
}

void NodeGroup::addNode(BluetoothNode *node)
{
    nodeMap.insert(node->getBluetoothAddress(), node);
}

void NodeGroup::removeNode(quint8 nodeAddr)
{
    nodeMap.remove(nodeAddr);
}

void NodeGroup::setName(const QString &name)
{
    this->name=name;
}

QString NodeGroup::getName()
{
    return this->name;
}

void NodeGroup::setId(quint8 id)
{
    this->id=id;
}

quint8 NodeGroup::getId()
{
    return this->id;
}

void NodeGroup::setColor(const QString &color)
{
    this->color=color;
}

QString NodeGroup::getColor()
{
    return this->color;
}

int NodeGroup::count()
{
    return nodeMap.size();
}

void NodeGroup::setBrightness(quint8 brightness)
{
    this->brightness=brightness;
}

quint8 NodeGroup::getBrightness()
{
    return this->brightness;
}

void NodeGroup::setScheduleType(NodeGroup::ScheduleType scheduleType)
{
    this->scheduleType=scheduleType;
}

NodeGroup::ScheduleType NodeGroup::getScheduleType()
{
    return this->scheduleType;
}

void NodeGroup::loadParamsValueByGroupData(const QString &groupData)
{
    QJsonObject dataJson=Tools::getJsonObjectFromDataArray(groupData.toUtf8());
    setScheduleType(this->getScheduleTypeByText(dataJson.value("schedule_type").toString()));
    setGroupType(this->getGroupTypeByText(dataJson.value("group_type").toString()));
}

QString NodeGroup::buildGroupData()
{
    QJsonObject dataJson;
    dataJson.insert("schedule_type", this->getTextByScheduleType(this->getScheduleType()));
    dataJson.insert("group_type", this->getTextByGroupType(this->getGroupType()));
    return QString::fromUtf8(Tools::getDataArrayOfJsonObject(&dataJson));
}


NodeGroup::ScheduleType NodeGroup::getScheduleTypeByText(const QString &scheduleTypeText)
{
    if(scheduleTypeText=="schedule_off")
    {
        return ScheduleOff;
    }
    else if(scheduleTypeText=="schedule_on_duty")
    {
        return ScheduleByOnDuty;
    }
    else if(scheduleTypeText=="schedule_off_duty")
    {
        return ScheduleByOffDuty;
    }
    else
    {
        return ScheduleByDate;
    }
}

QString NodeGroup::getTextByScheduleType(NodeGroup::ScheduleType scheduleType)
{
    if(scheduleType==ScheduleByOnDuty)
    {
        return "schedule_on_duty";
    }
    else if(scheduleType==ScheduleByOffDuty)
    {
        return "schedule_off_duty";
    }
    else if(scheduleType==ScheduleByDate)
    {
        return "schedule_date";
    }
    else
    {
        return "schedule_off";
    }
}

QList<QString> NodeGroup::getScheduleTypeTextList()
{
    QList<QString> list;
    list.push_back("schedule_off");
    list.push_back("schedule_date");
    list.push_back("schedule_on_duty");
    list.push_back("schedule_off_duty");
    return list;
}

NodeGroup::GroupType NodeGroup::getGroupTypeByText(const QString &groupTypeText)
{
    if(groupTypeText=="relay_group")
    {
        return RelayGroup;
    }
    else if(groupTypeText=="adboard_group")
    {
        return ADBoardGroup;
    }
    else
    {
        return LuminaireGroup;
    }
}

QString NodeGroup::getTextByGroupType(NodeGroup::GroupType groupType)
{
    if(groupType==RelayGroup)
    {
        return "relay_group";
    }
    else if(groupType==ADBoardGroup)
    {
        return "adboard_group";
    }
    else
    {
        return "luminaire_group";
    }
}

QList<QString> NodeGroup::getGroupTypeTextList()
{
    QList<QString> list;
    list.push_back("luminaire_group");
    list.push_back("relay_group");
    list.push_back("adboard_group");
    return list;
}

bool NodeGroup::isScheduleOn()
{
    return (getScheduleType()!=ScheduleOff);
}

NodeGroup *NodeGroup::copyAll()
{
    NodeGroup *group=new NodeGroup;
    group->setId(this->getId());
    group->setName(this->getName());
    group->setColor(this->getColor());
    group->setBrightness(this->getBrightness());
    group->setScheduleType(this->getScheduleType());
    group->setGroupType(this->getGroupType());
    return group;
}

NodeGroup::GroupType NodeGroup::getGroupType() const
{
    return m_groupType;
}

void NodeGroup::setGroupType(const GroupType &groupType)
{
    m_groupType = groupType;
}

bool NodeGroup::hasSpecialControl()
{
    return m_groupType==ADBoardGroup;
}


