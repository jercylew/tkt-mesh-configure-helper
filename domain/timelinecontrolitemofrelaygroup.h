#ifndef TIMELINECONTROLITEMOFRELAYGROUP_H
#define TIMELINECONTROLITEMOFRELAYGROUP_H

#include "timelinecontrolitem.h"

class TimeLineControlItemOfRelayGroup : public TimeLineControlItem
{
public:
    TimeLineControlItemOfRelayGroup();

    TimeLineControlItem *copyAll();
    void loadConfigData(TimeLineControlItem *controlItem);
    void loadAll(TimeLineControlItem *controlItem);
    NodeGroup::GroupType groupType();
};

#endif // TIMELINECONTROLITEMOFRELAYGROUP_H
