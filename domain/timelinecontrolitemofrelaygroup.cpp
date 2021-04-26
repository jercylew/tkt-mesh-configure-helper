#include "timelinecontrolitemofrelaygroup.h"

TimeLineControlItemOfRelayGroup::TimeLineControlItemOfRelayGroup()
{

}

TimeLineControlItem *TimeLineControlItemOfRelayGroup::copyAll()
{
    TimeLineControlItemOfRelayGroup *newControlItem=new TimeLineControlItemOfRelayGroup;
    newControlItem->loadAll(this);
    return newControlItem;
}

void TimeLineControlItemOfRelayGroup::loadConfigData(TimeLineControlItem *controlItem)
{
    TimeLineControlItem::loadConfigData(controlItem);
}

void TimeLineControlItemOfRelayGroup::loadAll(TimeLineControlItem *controlItem)
{
    TimeLineControlItem::loadAll(controlItem);
}

NodeGroup::GroupType TimeLineControlItemOfRelayGroup::groupType()
{
    return NodeGroup::RelayGroup;
}


